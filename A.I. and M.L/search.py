"""
Generic Search Algorithms, to help pacman find an optimal route to take
"""

def depthFirstSearch(problem):
    def createResult(reached):  # creates list of directions from the correct path of nodes
        result = []
        for index in range(len(reached) - 1):
            if(reached[index][0] - reached[index + 1][0] == 1):
                result.append('West')
            elif (reached[index][0] - reached[index + 1][0] == -1):
                result.append('East')
            elif (reached[index][1] - reached[index + 1][1] == 1):
                result.append('South')
            elif (reached[index][1] - reached[index + 1][1] == -1):
                result.append('North')

        return result

    def nextTo(start, end):  # Checker Function to let us know when a path can connect start->end
        xChange = start[0] - end[0]
        yChange = start[1] - end[1]
        if (xChange == 1 or xChange == -1) and yChange == 0:
            return True
        elif (yChange == 1 or yChange == -1) and xChange == 0:
            return True
        return False

    def expand(problem, node):  # return newly discovered nodes from the given one
        t = []
        s = []
        s = node

        for x in problem.successorStates(s):
            if(x[0] not in reached):
                t.append(x)
        return t

    def newPath(path, node):  # peel off unneeded coords to make new path
        done = False

        while done is False and len(path) > 0:
            if nextTo(node, path[-1]):
                path.append(node)
                done = True
            else:
                path.pop()
        return path

    result = []
    frontier = []
    reached = []
    node = []
    temp = []
    windex = 0
    failedPath = False

    result.append([])
    reached.append([problem.startingState()])

    # Set start node to "found" and add it's children to frontier
    for x in problem.successorStates(problem.startingState()):
        frontier.append(x)

    while len(frontier) > 0:  # Start Pathfinding!
        node.clear()
        temp.clear()
        failedPath = True

        node.append(frontier.pop())  # ---current node we look at---

        index = 0
        for x in reached:  # -----No new path needed-----

            if nextTo(x[-1], node[0][0]) and node[0][0] not in reached:
                windex = index
                reached[index].append(node[0][0])
                failedPath = False

                if problem.isGoal(node[0][0]):
                    return createResult(reached[windex])

            index += 1

        if failedPath is True:  # -----New path needed-----
            q = 0
            while q < len(reached) and failedPath is True:

                PH = newPath(list(reached[q]), node[0][0])
                if len(PH) > 0:
                    failedPath = False
                    reached.append(PH)

                q += 1

        for x in expand(problem, node[0][0]):  # ---look at nodes connected to current one. ---
            # ---If we havnt seen it before (reached or frontier) then add to frontier---
            if not any(x[0] in i for i in reached):
                if not any(x[0] in i for i in frontier):
                    frontier.append(x)

def breadthFirstSearch(problem):
    def createResult(reached):  # creates list of directions from the correct path of nodes
        result = []
        for index in range(len(reached) - 1):
            if(reached[index][0] - reached[index + 1][0] == 1):
                result.append('West')
            elif (reached[index][0] - reached[index + 1][0] == -1):
                result.append('East')
            elif (reached[index][1] - reached[index + 1][1] == 1):
                result.append('South')
            elif (reached[index][1] - reached[index + 1][1] == -1):
                result.append('North')

        return result

    def nextTo(start, end):  # Checker Function to let us know when a path can connect start->end
        xChange = start[0] - end[0]
        yChange = start[1] - end[1]
        if (xChange == 1 or xChange == -1) and yChange == 0:
            return True
        elif (yChange == 1 or yChange == -1) and xChange == 0:
            return True
        return False

    def expand(problem, node):  # return newly discovered nodes from the given one
        t = []
        s = []
        s = node

        for x in problem.successorStates(s):
            if(x[0] not in reached):
                t.append(x)
        return t

    def newPath(path, node):  # peel off unneeded coords to make new path
        done = False

        while done is False and len(path) > 0:

            if nextTo(node, path[-1]):
                path.append(node)
                done = True
            else:
                path.pop()

        return path

    result = []
    frontier = []
    reached = []
    node = []
    temp = []
    windex = 0
    failedPath = False

    result.append([])
    reached.append([problem.startingState()])

    # Set start node to "found" and add it's children to frontier
    for x in problem.successorStates(problem.startingState()):
        frontier.append(x)

    while len(frontier) > 0:  # Start Pathfinding!
        node.clear()
        temp.clear()
        failedPath = True

        node.append(frontier.pop(0))  # ---current node we look at---
        index = 0
        
        for x in reached:  # -----No new path needed-----

            if nextTo(x[-1], node[0][0]) and node[0][0] not in reached:
                windex = index
                reached[index].append(node[0][0])
                failedPath = False

                if problem.isGoal(node[0][0]):
                    return createResult(reached[windex])

            index += 1

        if failedPath is True:  # -----New path needed-----
            q = 0
            while q < len(reached) and failedPath is True:

                PH = newPath(list(reached[q]), node[0][0])
                if len(PH) > 0:
                    failedPath = False
                    reached.append(PH)

                q += 1

        for x in expand(problem, node[0][0]):  # ---look at nodes connected to current one. ---
            # ---If we havnt seen it before (reached or frontier) then add to frontier---
            if not any(x[0] in i for i in reached):
                if not any(x[0] in i for i in frontier):
                    frontier.append(x)

def uniformCostSearch(problem):
    """
    Search the node of least total cost first.
    """
    def createResult(reached):  # creates list of directions from the correct path of nodes
        result = []
        for index in range(len(reached) - 1):
            if(reached[index][0] - reached[index + 1][0] == 1):
                result.append('West')
            elif (reached[index][0] - reached[index + 1][0] == -1):
                result.append('East')
            elif (reached[index][1] - reached[index + 1][1] == 1):
                result.append('South')
            elif (reached[index][1] - reached[index + 1][1] == -1):
                result.append('North')

        return result

    def expand(problem, node):  # return newly discovered nodes from the given one
        t = []
        s = []
        s = node

        for x in problem.successorStates(s):
            if(x[0] not in reached):
                t.append(x)
        return t

    def newPath(path, node):  # peel off unneeded coords to make new path
        done = False

        while done is False and len(path) > 0:

            if any(node in i for i in problem.successorStates(path[-1])):
                path.append(node)
                done = True
            else:
                path.pop()

        return path

    result = []
    frontier = []
    reached = []
    node = []
    temp = []
    windex = 0
    failedPath = False

    result.append([])
    reached.append([problem.startingState()])

    # Set start node to "found" and add it's children to frontier
    for x in problem.successorStates(problem.startingState()):
        frontier.append(x)

    while len(frontier) > 0:

        node.clear()
        temp.clear()
        failedPath = True

        minCost = 9999
        mindex = 0

        for index in range(len(frontier)):  # ---choose the lowest cost action from our frontier---

            if frontier[index][2] < minCost:
                minCost = frontier[index][2]
                mindex = index

        node.append(frontier.pop(mindex))  # ---current node we look at---

        index = 0
        for x in reached:  # -----No new path needed-----

            if any(node[0][0] in i for i in problem.successorStates(x[-1])) and node[0][0] not in reached:
                windex = index
                reached[index].append(node[0][0])
                failedPath = False

                if problem.isGoal(node[0][0]):
                    return createResult(reached[windex])

            index += 1

        if failedPath is True:  # -----New path needed-----
            q = 0
            while q < len(reached) and failedPath is True:

                PH = newPath(list(reached[q]), node[0][0])
                if len(PH) > 0:
                    failedPath = False
                    reached.append(PH)

                q += 1

        for x in expand(problem, node[0][0]):  # ---look at nodes connected to current one. ---
            # ---If we havnt seen it before (reached or frontier) then add to frontier---
            if not any(x[0] in i for i in reached):
                if not any(x[0] in i for i in frontier):
                    frontier.append(x)

    return createResult(reached[windex])

def aStarSearch(problem, heuristic):
    def createResult(reached):  # creates list of directions from the correct path of nodes
        result = []
        for index in range(len(reached) - 1):
            if(reached[index][0] - reached[index + 1][0] == 1):
                result.append('West')
            elif (reached[index][0] - reached[index + 1][0] == -1):
                result.append('East')
            elif (reached[index][1] - reached[index + 1][1] == 1):
                result.append('South')
            elif (reached[index][1] - reached[index + 1][1] == -1):
                result.append('North')

        return result

    def nextTo(start, end):  # Checker Function to let us know when a path can connect start->end
        xChange = start[0] - end[0]
        yChange = start[1] - end[1]
        if (xChange == 1 or xChange == -1) and yChange == 0:
            return True
        elif (yChange == 1 or yChange == -1) and xChange == 0:
            return True
        return False

    def expand(problem, node):  # return newly discovered nodes from the given one
        t = []
        s = []
        s = node

        for x in problem.successorStates(s):
            if(x[0] not in reached):
                t.append(x)
        return t

    def newPath(path, node):  # peel off unneeded coords to make new path
        done = False

        while done is False and len(path) > 0:

            if nextTo(node, path[-1]):
                path.append(node)
                done = True
            else:
                path.pop()

        return path

    result = []
    frontier = []
    reached = []
    node = []
    temp = []
    windex = 0
    failedPath = False

    result.append([])
    reached.append([problem.startingState()])
    
    # Set start node to "found" and add it's children to frontier
    for x in problem.successorStates(problem.startingState()):
        frontier.append(x)

    while len(frontier) > 0:  # Start Pathfinding!

        node.clear()
        temp.clear()
        failedPath = True

        minCost = 9999
        mindex = 0

        for index in range(len(frontier)):  # Uses Lowest Cost to Pop
            tempCost = frontier[index][2] + heuristic(frontier[index][0], problem)
            if tempCost < minCost:
                minCost = tempCost
                mindex = index

        node.append(frontier.pop(mindex))  # ---current node we look at---

        index = 0
        for x in reached:  # -----No new path needed-----

            if nextTo(x[-1], node[0][0]) and node[0][0] not in reached:
                windex = index
                reached[index].append(node[0][0])
                failedPath = False

                if problem.isGoal(node[0][0]):
                    return createResult(reached[windex])

            index += 1

        if failedPath is True:  # -----New path needed-----
            q = 0
            while q < len(reached) and failedPath is True:

                PH = newPath(list(reached[q]), node[0][0])
                if len(PH) > 0:
                    failedPath = False
                    reached.append(PH)

                q += 1

        for x in expand(problem, node[0][0]):  # ---look at nodes connected to current one. ---
            # ---If we havnt seen it before (reached or frontier) then add to frontier---
            if not any(x[0] in i for i in reached):
                if not any(x[0] in i for i in frontier):
                    frontier.append(x)
