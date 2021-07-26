# KEY VALUE STORE
import os
import sys
import time
import requests
from flask import Flask, request, jsonify, make_response

app = Flask(__name__)

shard-id = -1 # A shard-id of -1 means unassigned
kvs = {}
vc = {}
for replica in os.getenv('VIEW').split(','):
    vc[replica] = 0
global_timeout = 0.5
socket_address = os.getenv('SOCKET_ADDRESS')

print(f'Socket address {socket_address}.', file=sys.stderr)
print(f'Initial view {list(vc.keys())}.', file=sys.stderr)

def reShard(shardCount):

    # ~~ Treat this as pseudocode!! ~~
    # It should work as is, but I'll do actual testing once the rest of the basic shard operations are in.

    # Questions/Assumptions:
    # Syntax for dict to dict assignment: add {key : value} to totalKVS is totalKVS[key] = node.kvs[key]
    # Pulling all nodes for the reShard using os.getenv('VIEW').split(',') vs. pinging all and using the nodes that respond?

    # Step 1: Pull data from old nodes into totalKVS
    pulledShards = []
    totalKVS = {}
    for node in os.getenv('VIEW').split(','):
        if node.shard-id in pulledShards: # Don't need to pull data from a shard more than once
            continue                      #  - Unless we pull from a 'bad' shard first?
        else:
            for key in node.kvs: # Pull each key/value into our new mega-dict
                totalKVS[key] = node.kvs[key]
            pulledShards.append(node.shard-id) # Mark the shard as done

    # Step 2: Gather a list of useable nodes into openNodes
    openNodes = os.getenv('VIEW').split(',') # Should we ping all nodes prior to assuming they are up?
    totalNodes = len(openNodes)

    # Step 3: Nodes per Shard/Assigning shard-id
    nodesPerShard = totalNodes//shardCount # Finds the floor, extras aren't used.
    if nodesPerShard < 2: # Enforce a minimum of 2 nodes per shard
        return jsonify(message="Not enough nodes to provide fault-tolerance with the given shard count!"), 400
    else:
        assignShardID = 1 # Start shard-ids at 1, could be 0 I suppose
        index = 0 # Keep track of how many nodes have been assigned a particular shard-id
        for node in openNodes:
            if index == nodesPerShard: # We assigned all nodes for a shard, so roll over to next shard
                assignShardID = assignShardID + 1
            node.shard-id = assignShardID # Assign the ID
            index = index + 1

    # Step 4: Re-distribute the data to our new shards
    # SOURCE for ascii value command - https://stackoverflow.com/questions/227459/how-to-get-the-ascii-value-of-a-character
    for key in totalKVS:
        shard = 0
        for x in range(len(key)):
            shard = shard + ord(key[x]) # Sum up ASCII value of each char in key
        shard = shard % shardCount # This key belongs to shard-id shard (after the mod is performed)
        for node in openNodes: # Go through our nodes, and place the key/value in all nodes with the right shard-id
            if node.shard-id == shard:
                node.kvs[key] = totalKVS[key] # Python syntax is weird

    return jsonify(message="Resharding done succesfully"), 200 # Done!

# 1 -> vc1 is >, -1 -> vc2 is >, 0 -> vc1 == vc2, None -> can't tell
def compare_VCs(vc1, vc2):
    left_greater = False
    right_greater = False
    intersect_vc = vc1.keys() & vc2.keys()
    if len(intersect_vc) == 0:
        return None
    for key in intersect_vc:
        value1 = vc1[key]
        value2 = vc2[key]
        if value1 > value2:
            left_greater = True
        elif value1 < value2:
            right_greater = True
    if left_greater is True and right_greater is True:
        return None
    elif left_greater is True and right_greater is False:
        return 1
    elif left_greater is False and right_greater is True:
        return -1
    else:
        return 0


def broadcast_view_change(change_type, data):
    print(f'In view change.', file=sys.stderr)
    if change_type == "PUT":
        for replica in vc.copy():
                try:
                    requests.put(f'http://{replica}/key-value-store-view', json={'socket-address': data},
                                 timeout=global_timeout)
                except:
                    continue
    else:
        for replica in vc.copy():
            try:
                requests.delete(f'http://{replica}/key-value-store-view', json={'socket-address': data},
                                timeout=global_timeout)
            except:
                continue


@app.route('/key-value-store-view', methods=['GET', 'PUT', 'DELETE'])
def key_value_store_view():
    if request.method == 'GET':
        return jsonify(message="Retrieved successfully", view=','.join(vc.keys())), 200
    elif request.method == 'PUT':
        replica = request.get_json()['socket-address']
        if replica not in vc.copy():
            vc[replica] = 0
            return jsonify(message="Replica added successfully to the view"), 200
        else:
            return jsonify(error="Socket address already exists in the view", message="Error in PUT"), 404
    else:
        replica = request.get_json()['socket-address']
        if replica in vc.copy():
            del vc[replica]
            return jsonify(message="Replica deleted successfully from the view"), 200
        else:
            return jsonify(error="Socket address does not exist in the view", message="Error in DELETE"), 404


@app.route('/server-kvs', methods=['GET'])
@app.route('/server-kvs/<key>', methods=['PUT'])
def server_kvs(key=None):
    if request.method == 'GET':
        return jsonify(kvs=kvs, vc=vc), 200
    else:
        try:
            value = request.get_json()['value']
        except:
            return jsonify(error="Value is missing"), 500
        try:
            causal_metadata = request.get_json()['causal-metadata']
            if causal_metadata != "" and compare_VCs(vc, causal_metadata) == -1:
                server_gossip(causal_metadata)
        except:
            return jsonify(error="Causal metadata is missing"), 500
        try:
            server = request.get_json()['server']
        except:
            return jsonify(error="Server is missing"), 500
        vc[server] += 1
        kvs[key] = value
        print(f'Just added {key}.', file=sys.stderr)
        return jsonify(message="Successfully received broadcast"), 200


def server_gossip(view):
    global vc
    global kvs
    broadcast_view_change("PUT", socket_address)
    for replica in view:
        if replica != socket_address:
            try:
                response = requests.get(f'http://{replica}/server-kvs', timeout=global_timeout)
                replica_vc = response.json()['vc']
                vc_comp = compare_VCs(vc, replica_vc)
                if vc_comp == -1:
                    vc = replica_vc
                    kvs = response.json()['kvs']
            except:
                broadcast_view_change("DELETE", replica)


def broadcast_key_change(key, causal_metadata):
    # FORWARDING TO REPLICAS
    for replica in vc.copy():
        if replica != socket_address:
            try:
                print(f'Sending to {replica}.', file=sys.stderr)
                requests.put(f'http://{replica}/server-kvs/{key}',
                             json={'value': kvs[key],
                                   'causal-metadata': causal_metadata,
                                   'server': socket_address}, timeout=global_timeout)
            except:
                print(f'{replica} server is dead', file=sys.stderr)
                broadcast_view_change('DELETE', replica)


@app.route('/key-value-store/<key>', methods=['GET', 'PUT', 'DELETE'])
def key_value_store(key):
    global vc
    if request.method == 'GET':
        if key in kvs:
            return make_response(jsonify(
                {"message": "Retrieved successfully", "causal-metadata": vc, "value": kvs[key]}
            ), 200)
        else:
            return jsonify(error="Key does not exist", message="Error in GET"), 404

    elif request.method == 'PUT':
        try:
            value = request.get_json()['value']
        except:
            return jsonify(error="Value is missing", message="Error in PUT"), 400
        try:
            causal_metadata = request.get_json()['causal-metadata']
            if causal_metadata != "" and compare_VCs(vc, causal_metadata) == -1:
                server_gossip(causal_metadata)
        except:
            return jsonify(error="Causal metadata is missing", message="Error in PUT"), 400
        if len(key) > 50:
            return jsonify(error="Key is too long", message="Error in PUT"), 400
        elif key not in kvs:
            message = "Added successfully"
            status_code = 201
        else:
            message = "Updated successfully"
            status_code = 200
        vc[socket_address] += 1
        kvs[key] = value
        print(f'Just added {key}.', file=sys.stderr)
        broadcast_key_change(key, causal_metadata)
        return make_response(jsonify(
            {"message": message, "causal-metadata": vc}
        ), status_code)
    else:
        try:
            causal_metadata = request.get_json()['causal-metadata']
            if causal_metadata == "":
                return jsonify(error="Key does not exist", message="Error in DELETE"), 404
            else:
                if compare_VCs(vc, causal_metadata) == -1:
                    server_gossip(causal_metadata)
        except:
            return jsonify(error="Causal metadata is missing", message="Error in DELETE"), 400
        if key in kvs:
            vc[server] += 1
            kvs[key] = None
            broadcast_key_change(key, causal_metadata)
            return make_response(jsonify(
                {"message": "Deleted successfully", "causal-metadata": vc}
            ), 200)
        else:
            return jsonify(error="Key does not exist", message="Error in DELETE"), 404


server_gossip(vc.copy())
if __name__ == "__main__":
    # this is ignored when run in docker, but is useful for quicker testing outside of docker
    app.run(host="127.0.0.1", port=8085, debug=True, threaded=True)
