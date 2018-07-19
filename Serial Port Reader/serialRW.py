import sys
import serial
import time
import os
import serial.tools.list_ports
import datetime
import glob

class serialRW:
    def __init__(self, serialPort = None, baudRate = None):
        self.sendString = ""
        self.serialPort = serialPort
        self.baudRate = 0
        self.sp = serial.Serial()
        if serialPort != None and baudRate != None:
            self.baudRate = self.parseBaudRate(baudRate)
            self.openPort()
    
    def closePort(self):
        self.sp.close()
    
    def openPort(self):
        listOfPorts = self.listOfAvailableSerialPorts()
        if len(listOfPorts) == 0:
            print("There is no available Serial Ports!")
            sys.exit()
        for lst in listOfPorts:
            if self.serialPort in lst:
                #print(lst)
                break
            else:
                print("Serial port " + self.serialPort + " is not available!")
                sys.exit()
        try:
            os.system("sudo chmod 666 " + self.serialPort)
        except os.error as e:
            print("Error: " + str(e))
        self.sp.port = self.serialPort
        self.sp.baudrate = self.parseBaudRate(self.baudRate)
        self.sp.bytesize = serial.EIGHTBITS
        self.sp.parity = serial.PARITY_NONE
        self.sp.stopbits = serial.STOPBITS_ONE
        self.sp.timeout = 1
        self.sp.xonxoff = False
        self.sp.dsrdtr = False
        self.sp.writeTimeout = 2
        self.sp.open()
        if self.sp.isOpen():
            listOfFiles = glob.glob("*.txt")
            fileName = "serialData-" + str(datetime.datetime.now().strftime("%Y-%m-%d"))
            for lst in listOfFiles:
                if fileName in lst:
                    fileName = fileName + "(1)"
            fileName = fileName + ".txt"
            self.file = open(fileName, "a")


    def listOfAvailableSerialPorts(self):
        listOfCOMPorts = serial.tools.list_ports_linux.comports()
        result = []
        for lst in listOfCOMPorts:
            if "USB" in lst[0]:
                result.append(lst[0])
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
            try:
                self.sp.flushInput()
                self.sp.flushOutput()
                received = self.sp.readline()
                if received and received.strip():
                    forPrint = self.checkReceivedMessage(received)
                    self.file.write(forPrint + "\n")
                    return forPrint
            except Exception as e:
                print ("Error: " + str(e))
        else:
            print("Cannot open serial port: " + self.serialPort)
            sys.exit()
        
    def isOpen(self):
        return self.sp.isOpen()
    
    def setSendString(self, string):
        self.sp.flushInput()
        self.sp.flushOutput()
        if len(string) > 0:
            sendString = string + "\r"
            self.sp.write(sendString.encode())


                    

    
