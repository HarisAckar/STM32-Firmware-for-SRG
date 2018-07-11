import sys
import serial
import time
import os
import serial.tools.list_ports
import datetime
import glob

class serialRW:
    def __init__(self, serialPort, baudRate):
        self.sendString = ""
        self.serialPort = serialPort
        self.baudRate = self.parseBaudRate(baudRate)
        self.sp = serial.Serial()
        listOfPorts = self.listOfAvailableSerialPorts()
        if len(listOfPorts) == 0:
            print("There is no available Serial Ports!")
            sys.exit()
        for lst in listOfPorts:
            if serialPort in lst[0]:
                print(lst)
                break
            else:
                print("Serial port " + serialPort + " is not available!")
                sys.exit()
        try:
            os.system("sudo chmod 666 /dev/" + serialPort)
        except os.error as e:
            print("Error: " + str(e))
        self.sp.port = "/dev/" + serialPort
        self.sp.baudrate = self.parseBaudRate(baudRate)
        self.sp.bytesize = serial.EIGHTBITS
        self.sp.parity = serial.PARITY_NONE
        self.sp.stopbits = serial.STOPBITS_ONE
        self.sp.timeout = 1
        self.sp.xonxoff = False
        self.sp.dsrdtr = False
        self.sp.writeTimeout = 2
        self.sp.open()

    def listOfAvailableSerialPorts(self):
        listOfCOMPorts = serial.tools.list_ports.comports()
        result = []
        for lst in listOfCOMPorts:
            if "USB" in lst[0]:
                result.append(lst)
        return result

    def parseBaudRate(self, string):
        try:
            result = int(string)
            return result
        except Exception:
            print("Invalid baud rate!")
            sys.exit(-1)
    
    def checkReceivedMessage(self, received):
        flag = False
        filteredString = ''
        for r in received:
            if(chr(r) != '!' and not flag):
                continue
            elif(chr(r) == '!' and not flag):
                flag = True
                filteredString = str(chr(r))
            elif(chr(r) != '!' and chr(r) != '\r' and flag):
                filteredString = filteredString + str(chr(r))
            elif(chr(r) == '\r'):
                break
        return filteredString
    
    def getMessage(self):
        if self.sp.isOpen():
            listOfFiles = glob.glob("*.txt")
            fileName = "serialData-" + str(datetime.datetime.now().strftime("%Y-%m-%d"))
            for lst in listOfFiles:
                if fileName in lst:
                    fileName = fileName + "(1)"
            fileName = fileName + ".txt"
            file = open(fileName, "a")
            try:
                self.sp.flushInput()
                self.sp.flushOutput()
                time.sleep(0.5)
                while self.sp.isOpen:
                    received = self.sp.readline()
                    if received and received.strip():
                        forPrint = self.checkReceivedMessage(received)
                        file.write(forPrint + "\n")
                    if len(self.sendString) > 0:
                        self.sp.write(self.sendString + "\r\n")
                        self.sendString = ""
            except Exception as e:
                print ("Error: " + str(e))
        else:
            print("Cannot open serial port: " + self.serialPort)
            sys.exit()
    
    def setSendString(self, string):
        self.sendString = string

s = serialRW("ttyUSB0", "115200")
s.getMessage()
                    

    
