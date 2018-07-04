# Author: Haris Ackar
# TODO
import sys
import serial
import time
import os
import serial.tools.list_ports
import signal
import glob
import datetime


def signalHandler(signal, frame):
    sys.exit()

def listOfAvailableSerialPorts():
    listOfCOMPorts = serial.tools.list_ports.comports()
    result = []
    for lst in listOfCOMPorts:
        if "USB" in lst[0]:
            result.append(lst)
    return result

def openSerialPort(serialPort, baudRate):
    listOfPorts = listOfAvailableSerialPorts()
    if len(listOfPorts) == 0:
        print "There is no available Serial Ports!"
        sys.exit()
    for lst in listOfPorts:
        if serialPort in lst[0]:
            print(lst)
            break
        else:
            print "Serial port " + serialPort + " is not available!"
            sys.exit()
    try:
        os.system("sudo chmod 666 /dev/" + serialPort)
    except os.error, e:
        print "Error: " + str(e)
    sp = serial.Serial()
    sp.port = "/dev/" + serialPort
    sp.baudrate = baudRate
    sp.bytesize = serial.EIGHTBITS
    sp.parity = serial.PARITY_NONE
    sp.stopbits = serial.STOPBITS_ONE
    sp.timeout = 1
    sp.xonxoff = False
    sp.dsrdtr = False
    sp.writeTimeout = 2
    sp.open()

    if sp.isOpen():
        print "Serial port: " + serialPort + ", Baud Rate: " + str(baudRate) + ":"
        listOfFiles = glob.glob("*.txt")
        fileName = "serialdata-" + str(datetime.datetime.now().strftime("%Y-%m-%d"))
        for lst in listOfFiles:
            if fileName in lst:
                fileName = fileName + "(1)"
        file = open(fileName, "a")
        try:
            sp.flushInput()
            sp.flushOutput()
            time.sleep(0.5)
            while sp.isOpen:
                received = sp.readline()
                if received and received.strip():
                    print received
                    file.write(received + "\r\n")
        except Exception, e:
            print "Error: " + str(e)
    else:
        print "Cannot open serial port" + serialPort
        sys.exit()

def parseBaudRate(string):
    try:
        result = int(string)
        return result
    except Exception:
        print "Invalid baud rate!"
        sys.exit(-1)

def main(argv):
    serialPort = ''
    if len(argv) == 1:
        print "Not enough input arguments"
        sys.exit()
    elif len(argv) == 2:
        if argv[1] == "-h":
            print "$main.py -i <serialPort> -i <baudRate> #for reading from Serial Port"
            print "$main.py -l #for list of all available Serial Ports"
            print "$main.py -h #for help"
            sys.exit();
        elif argv[1] == "-l":
            print(listOfAvailableSerialPorts())
            sys.exit()
    elif len(argv) == 3:
        serialPort = argv[1]
        baudRateINT = parseBaudRate(argv[2])
        openSerialPort(serialPort, baudRateINT)
    else:
        print "Too many input arguments!"
        sys.exit()

if __name__ == "__main__":
    os.system("clear")
    signal.signal(signal.SIGINT, signalHandler)
    main(sys.argv)