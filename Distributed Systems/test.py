################### 
# Course: CSE138
# Date: Spring 2021
# Assignment: 2
# Authors: Reza NasiriGerdeh, Lindsey Kuper, Patrick Redmond
# This document is the copyrighted intellectual property of the authors.
# Do not copy or distribute in any form without explicit permission.
# MODIFICATION: For personal testing purposes, this document has been 
# modified by Kyle Serio, and creates a multi-purpose program
# to create server instances as well as test them, basing these
# actions off the code of the original authors work
###################

import unittest
import subprocess
import requests
import sys
import time

class Part1Const:
    mainInstanceName = "main-instance"
    hostPortMainInstance = "8085"
    hostname = 'localhost'  # Windows and Mac users can change this to the docker vm ip
    portNumber = '8085'
    ipAddressMainInstance = 'http://' + hostname + ":" + portNumber
    baseUrl = 'http://' + hostname + ":" + portNumber

class Part2Const:
    subnetName = "assignment2-net"
    subnetAddress = "10.10.0.0/16"

    mainInstanceName = "main-instance"
    forwardingInstance1Name = "forwarding-instance1"
    forwardingInstance2Name = "forwarding-instance2"

    hostname = 'localhost'  # Windows and Mac users can change this to the docker vm ip

    ipAddressMainInstance = "10.10.0.2"
    hostPortMainInstance = "8086"

    ipAddressForwarding1Instance = "10.10.0.3"
    hostPortForwarding1Instance = "8087"

    ipAddressForwarding2Instance = "10.10.0.4"
    hostPortForwarding2Instance = "8088"


# docker linux commands
#
# * all wait for subprocess to complete
# * all but `docker build` suppress stdout
# * all but the removal commands require success
# * the removal commands require success by default but have an argument which can be set to False to ignore failure
# * the removal commands additionally suppress stderr

def removeSubnet(subnetName, required=True):
    command = "docker network rm " + subnetName
    subprocess.run(command, shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, check=required)

def createSubnet(subnetAddress, subnetName):
    command  = "docker network create --subnet=" + subnetAddress + " " + subnetName
    subprocess.check_call(command, stdout=subprocess.DEVNULL, shell=True)

def buildDockerImage():
    command = "docker build -t assignment2-img ."
    subprocess.check_call(command, shell=True)

def runSoloInstance(hostPortNumber, ipAddress, instanceName):
    command = "docker run -p " + hostPortNumber + ":8085" + " --name=" + instanceName + " assignment2-img"
    subprocess.check_call(command, shell=True, stdout=subprocess.DEVNULL)

def runMainInstance(hostPortNumber, ipAddress, subnetName, instanceName):
    command = "docker run -p " + hostPortNumber + ":8085 --net=" + subnetName + " --ip=" + ipAddress + " --name=" + instanceName + " assignment2-img"
    print("COMMAND: " + command)
    subprocess.check_call(command, shell=True, stdout=subprocess.DEVNULL)

def runForwardingInstance(hostPortNumber, ipAddress, subnetName, instanceName, forwardingAddress):
    command = "docker run -p " + hostPortNumber + ":8085 --net=" + subnetName  + " --ip=" + ipAddress + " --name=" + instanceName + " -e FORWARDING_ADDRESS=" + forwardingAddress + " assignment2-img"
    print("COMMAND: " + command)
    subprocess.check_call(command, shell=True, stdout=subprocess.DEVNULL)

def stopAndRemoveInstance(instanceName, required=True):
    stopCommand = "docker stop " + instanceName
    subprocess.run(stopCommand, shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, check=required)
    time.sleep(1)
    removeCommand = "docker rm " + instanceName
    subprocess.run(removeCommand, shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, check=required)


class Part2(unittest.TestCase):
    ############ Test suite lifecycle: setUpClass [setUp test tearDown]* tearDownClass #########
    @classmethod
    def setUpClass(cls):
        print('= Cleaning-up containers and subnet, possibly leftover from a previous interrupted run..')
        stopAndRemoveInstance(Part2Const.mainInstanceName, required=False)
        stopAndRemoveInstance(Part2Const.forwardingInstance1Name, required=False)
        stopAndRemoveInstance(Part2Const.forwardingInstance2Name, required=False)
        # Prevent "Error response from daemon: error while removing network: network assignment2-net id ... has active endpoints"
        time.sleep(5)
        removeSubnet(Part2Const.subnetName, required=False)
        print('= Creating image and subnet..')
        buildDockerImage()
        createSubnet(Part2Const.subnetAddress, Part2Const.subnetName)

    def setUp(self):
        print("\n== Running containers..")
        runMainInstance(Part2Const.hostPortMainInstance, Part2Const.ipAddressMainInstance, Part2Const.subnetName, Part2Const.mainInstanceName)
        runForwardingInstance(Part2Const.hostPortForwarding1Instance, Part2Const.ipAddressForwarding1Instance, Part2Const.subnetName, Part2Const.forwardingInstance1Name, Part2Const.ipAddressMainInstance + ":8085" )
        runForwardingInstance(Part2Const.hostPortForwarding2Instance, Part2Const.ipAddressForwarding2Instance, Part2Const.subnetName, Part2Const.forwardingInstance2Name, Part2Const.ipAddressMainInstance + ":8085" )
        # Ensure that processes in the containers have time to start up and listen on ports
        time.sleep(5)

    def tearDown(self):
        print("== Destroying containers..")
        stopAndRemoveInstance(Part2Const.mainInstanceName)
        stopAndRemoveInstance(Part2Const.forwardingInstance1Name)
        stopAndRemoveInstance(Part2Const.forwardingInstance2Name)

    @classmethod
    def tearDownClass(cls):
        print('= Destroying subnet..')
        removeSubnet(Part2Const.subnetName)

#Spin up a Solo Server Using Static Values
def noForward():
    print('Starting solo server: Host Name: {2}, Host Port {0}, ip: {1}'.format(Part1Const.hostPortMainInstance, Part1Const.ipAddressMainInstance, Part1Const.mainInstanceName))
    stopAndRemoveInstance(Part1Const.mainInstanceName, required=False)
    time.sleep(5)
    buildDockerImage()
    runSoloInstance(Part1Const.hostPortMainInstance, Part1Const.ipAddressMainInstance, Part1Const.mainInstanceName)
    pause = ' '
    while pause != 'q':
        print("[q] to kill")
        print("Server Running...")
        pause = input()
    print("Server Stopping...")

#Spin up Main/Host Server for Forwarding 
def forwardMain(hostPort, hostIp, subnet, hostName):
    print('Starting main server: Host Name: {0}, Host Port: {1}, ip: {2}, subnet: {3}'.format(hostName, hostPort, hostIp, subnet))
    stopAndRemoveInstance(hostName, required=False)
    removeSubnet(subnet, required=False)
    time.sleep(5)
    buildDockerImage()
    createSubnet(Part2Const.subnetAddress, subnet)
    runMainInstance(hostPort, hostIp, subnet, hostName)
    pause = ' '
    while pause != 'q':
        print("[q] to kill")
        print("Host Server Running...")
        pause = input()
    print("Host Server Stopping...")
    stopAndRemoveInstance(hostName, required=False)
    removeSubnet(subnet, required=False)

#Spin up a Forwarding Instance
def forward(hostPort, forwardingAddress, subnet, instanceName, hostIp):
    print('Starting forwarding instance:Instance Name: {0}, Host Port: {1}, ip: {2}, subnet: {3}, Forwarding Address: {4}'.format(instanceName, hostPort, hostIp, subnet, forwardingAddress))
    stopAndRemoveInstance(instanceName, required=False)
    time.sleep(5)
    buildDockerImage()
    runForwardingInstance(hostPort, forwardingAddress, subnet, instanceName, hostIp)
    pause = ' '
    while pause != 'q':
        print("[q] to kill")
        print("Forwarding Server Running...")
        pause = input()
    print("Forwarding Server Stopping...")
    stopAndRemoveInstance(instanceName, required=False)

#Solo Server Test Menu
def noForwardMenu():
    task = ' '
    print("noForward Menu: \nEnter [g]et, [p]ut, or [d]elete followed by the key: ('gtest' sends a GET for key 'test')\n[i] To Repeat This Info\n[q] Return To Main Menu")
    while task != 'q':
        print("\nEnter Task:")
        task = input()
        if task == 'i':
            print(noForwardInfo)
        elif len(task) == 0:
            print("Need something to do!")
        elif len(task) == 1 and task != 'q':
            print("Task needs a key!")
        elif task[1:2] == ' ':
            print("Don't use spaces!")
        elif task[:1] == 'g':
            print("Sending a GET Request for '{}'...".format(task[1:]))
            response = requests.get(Part1Const.baseUrl + '/key-value-store/' + task[1:])
            responseInJson = response.json()
            print("Code   : {}".format(response.status_code))
            print("Exists?: {}".format(responseInJson['doesExist']))
            print("Message: {}".format(responseInJson['message']))
            try:
                print("Error! : {}".format(responseInJson['error']))
            except:
                print("Value  : {}".format(responseInJson['value']))
        elif task[:1] == 'd':
            print("Trying to DELETE '{}'...".format(task[1:]))
            response = requests.delete(Part1Const.baseUrl + '/key-value-store/' + task[1:])
            responseInJson = response.json()
            print("Code   : {}".format(response.status_code))
            print("Exists?: {}".format(responseInJson['doesExist']))
            print("Message: {}".format(responseInJson['message']))
            try:
                print("Error! : {}".format(responseInJson['error']))
            except:
                print()
        elif task[:1] == 'p':
            print("Key Given: {}, Please Enter Value...".format(task[1:]))
            value = input()
            print("Sending a PUT Request for '{}:{}'...".format(task[1:],value))
            response = requests.put(Part1Const.baseUrl + '/key-value-store/' + task[1:], json={'value': value})
            responseInJson = response.json()
            print("Code   : {}".format(response.status_code))
            print("Message: {}".format(responseInJson['message']))
            print("Replaced?: {}".format(responseInJson['replaced']))
        elif task != 'q':
            print("Task not recognized, try again!")

#Editing info for a main/host server prior to starting it   
def editHostInfo(hostName, hostPort, hostIp, subnet):
    edit = ' '
    while edit != 'q':
        print("EDIT HOST SERVER OPTIONS")
        print('Main Server Info is: \nHost Name: {3}, \nHost Port {0}, \nip: {1}, \nsubnet: {2}'.format(hostPort, hostIp, subnet, hostName))
        print('To Edit: \n[hn] Host Name, \n[hp] Host Port, \n[ip] IP Address, \n[sn] Subnet Name\n[q] Save Changes')
        edit = input()
        if edit == 'hn':
            print("Enter new host name: ")
            hostName = input()
        elif edit == 'hp':
            print("Enter new host port: ")
            hostPort = input()
        elif edit == 'ip':
            print("Enter new host ip: ")
            hostIp = input()
        elif edit == 'sn':
            print("Enter new subnet: ")
            subnet = input()        
        elif edit == 'q':
            return(hostName, hostPort, hostIp, subnet)
        else:
            print("Command Unknown")

#Editing info for a forwarding server prior to starting it 
def editForwardingInfo(hostPort, hostIp, subnet, instanceName, forwardingAddress):
    edit = ' '
    while edit != 'q':
        print("EDIT FORWARDING SERVER OPTIONS")
        ('FORWARDING SERVER INFO: \nPort         : {0} \nHost IP      : {1} \nInstance Name: {2} \nInstance IP  : {3}\nSubnet Name  : {4}'.format(hostPort, hostIp, instanceName, forwardingAddress, subnet))
        print('To Edit: \n[hp] Host Port \n[ip] Host IP\n[in] Instance Name \n[fa] Forwarding Address (this instance ip)\n[sn] Subnet Name or\n[q] Save Changes')
        edit = input()

        if edit == 'hp':
            print("Enter new host port: ")
            hostPort = input()
        elif edit == 'ip':
            print("Enter new host ip: ")
            hostIp = input()
        elif edit == 'sn':
            print("Enter new subnet: ")
            subnet = input()
        elif edit == 'in':
            print("Enter new Instance Name: ")
            instanceName = input()
        elif edit == 'fa':
            print("Enter new Forwarding Address: ")
            forwardingAddress = input()            
        elif edit == 'q':
            return(hostPort, hostIp, subnet, instanceName, forwardingAddress)
        else:
            print("Command Unknown")

def forwardMenu():
    hostName = Part2Const.mainInstanceName
    hostIp = Part2Const.ipAddressMainInstance
    subnet = Part2Const.subnetName
    hostPort = Part2Const.hostPortMainInstance
    baseUrl = 'http://' + 'localhost' + ":" + hostPort
    info = "Host Name: {} Host Port: {} Host IP: {} Subnet: {}".format(hostName, hostPort, hostIp, subnet)
    task = ' '
    print("FORWARDING MENU: - Pinging Port: {}\nEnter [g]et, [p]ut, or [d]elete followed by the key: ('gtest' sends a GET for key 'test')\n[e] To Edit Port \n[i] To Display Server Info, \n[q] To Return To Main Menu".format(hostPort))
    while task != 'q':
        print("\nEnter Task for Port {} ([e] To Change Port):".format(hostPort))
        task = input()
        if task == 'i':
            print(info)
        elif len(task) == 0:
            print("Need something to do!")
        elif len(task) == 1 and task != 'q' and task != 'e':
            print("Task needs a key!")
        elif task[1:2] == ' ':
            print("Don't use spaces!")
        elif task == 'e':
            print("Current Url: {}".format(baseUrl))
            print("Enter new port: ")
            hostPort = input()
            baseUrl = 'http://' + 'localhost' + ":" + hostPort
            print("New Url: {}".format(baseUrl))
        elif task[:1] == 'g':
            print("Sending a GET Request for '{}'...".format(task[1:]))
            response = requests.get(baseUrl + '/key-value-store/' + task[1:])
            responseInJson = response.json()
            print("Code   : {}".format(response.status_code))
            print("Exists?: {}".format(responseInJson['doesExist']))
            print("Message: {}".format(responseInJson['message']))
            try:
                print("Error! : {}".format(responseInJson['error']))
            except:
                print("Value  : {}".format(responseInJson['value']))
        elif task[:1] == 'd':
            print("Trying to DELETE '{}'...".format(task[1:]))
            response = requests.delete(baseUrl + '/key-value-store/' + task[1:])
            responseInJson = response.json()
            print("Code   : {}".format(response.status_code))
            print("Exists?: {}".format(responseInJson['doesExist']))
            print("Message: {}".format(responseInJson['message']))
            try:
                print("Error! : {}".format(responseInJson['error']))
            except:
                print()
        elif task[:1] == 'p':
            print("Key Given: {}, Please Enter Value...".format(task[1:]))
            value = input()
            print("Sending a PUT Request for '{}:{}'...".format(task[1:],value))
            response = requests.put(baseUrl + '/key-value-store/' + task[1:], json={'value': value})
            responseInJson = response.json()
            print("Code   : {}".format(response.status_code))
            print("Message: {}".format(responseInJson['message']))
            print("Replaced?: {}".format(responseInJson['replaced']))
        elif task != 'q':
            print("Task not recognized, try again!")

if __name__ == '__main__':
    info = "[nf]  Starts a Solo Server \n[nfm] To Test Solo \nForward Testing: \n[h]   To Start a Host/Main Server \n[f]   To Start a Forwarding Server \n[fm]  To Test Forwarding Session \n[i]   To Re-Print This Menu \n[q]   To Quit"
    if len(sys.argv) == 2:
        unittest.main(verbosity=2)

    task = ' '
    print("MAIN MENU: ")
    print(info)
    while task != 'q':
        print("Enter Task:")
        task = input()
        if task == 'i':
            print(info)
        elif task == 'nfm':
            noForwardMenu()
            print(info)
        elif task == 'nf':
            print('Clean up and Starting Solo Server, Values: \nHost Name: {2}, \nHost Port: {0}, \nIP: {1}'.format(Part1Const.hostPortMainInstance, Part1Const.ipAddressMainInstance, Part1Const.mainInstanceName))
            print("Press [Enter] To Begin, or [q] to abort. (Loading will pause, don't worry!)")
            pause = input()
            if pause != 'q':
                print("Loading...")
                noForward()
        elif task == 'h':
            hostName = Part2Const.mainInstanceName
            hostPort = Part2Const.hostPortMainInstance
            hostIp = Part2Const.ipAddressMainInstance
            subnet = Part2Const.subnetName
            print('Main Server Info: \nHost Name: {3} \nHost Port: {0} \nIP       : {1} \nSubnet Name: {2}'.format(hostPort, hostIp, subnet, hostName))
            edit = ' '
            while edit != 'q':
                print('[e] to edit prior to startup \n[q] to abort \n[enter] to proceed')
                edit = input()
                if edit == 'e':
                    hostName, hostPort, hostIp, subnet = editHostInfo(hostName, hostPort, hostIp, subnet)
                elif edit == 'q':
                    print()
                else:
                    forwardMain(hostPort, hostIp, subnet, hostName)
        elif task == 'f':
            hostPort = Part2Const.hostPortForwarding1Instance
            forwardingAddress = Part2Const.ipAddressForwarding1Instance
            subnet = Part2Const.subnetName
            instanceName = Part2Const.forwardingInstance1Name
            hostIp = Part2Const.ipAddressMainInstance + ":"
            print("What Number Forwarding Server Is This? (I.E. 1st, 2nd, etc)")
            num = input()
            num = int(num)
            while num < 1 or num > 16:
                print("Gave: {}, Please enter a number between 1-16".format(num))
                num = input()
                num = int(num)
            port = 8086 + num
            hostPort = str(port)
            ip = 2 + num
            forwardingAddress = forwardingAddress[:len(forwardingAddress)-1] + str(ip)
            instanceName = instanceName[:len(instanceName)-1] + str(num)
            print("Which port is this server connecting to? (8085/8086 for host, 8087 for 1st forwarding etc) (NOTE: I'm not sure we have this implemented yet, enter 8085 normally)")
            num = input()
            num = int(num)
            while num < 8085 or num > 8100:
                print("Gave: {}, Please enter a number between 8085-8100 (NOTE: I'm not sure we have this implemented yet, enter 8085 normally)".format(num))
                num = input()
                num = int(num)
            hostIp = hostIp + str(num)
            print('FORWARDING SERVER INFO: \nPort         : {0} \nHost IP      : {1} \nInstance Name: {2} \nInstance IP  : {3}\nSubnet Name  : {4}'.format(hostPort, hostIp, instanceName, forwardingAddress, subnet))
            edit = ' '
            while edit != 'q':
                print('[e] To Edit Prior to Startup \n[q] To Abort \n[enter] To Proceed')
                edit = input()
                if edit == 'e':
                    hostPort, hostIp, subnet, instanceName, forwardingAddress = editForwardingInfo(hostPort, hostIp, subnet, instanceName, forwardingAddress)
                elif edit == 'q':
                    print()
                else:
                    forward(hostPort, forwardingAddress, subnet, instanceName, hostIp)
        elif task == 'fm':
            forwardMenu()
            print(info)
        print("Main Menu: ")
