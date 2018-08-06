import threading
import serialRW
import time
import sys
from pyqtgraph.Qt import QtCore, QtGui
import matplotlib.pyplot as plt 
import matplotlib.animation as animation

class Aquisition (threading.Thread):
    def __init__(self, threadID, name, serialRW):
        threading.Thread.__init__(self)
        self.threadID = threadID
        self.name = name
        self.serial = serialRW
        self.dead = False
        self.fileName = "ADC_Voltage.txt"
        self.lineNumber = 0
    
    def run(self):
        self.readAquisition()
    
    def readAquisition(self):
        while not self.dead:
            data = self.serial.getMessage()
            if data != None:
                try:
                    value = float(data[1:])
                    value = value * 3 / 255
                    file = open(self.fileName, "a")
                    file.write(str(self.lineNumber) + ',' + str(value) + "\n")
                    file.close()
                    self.lineNumber = self.lineNumber + 1
                except:
                    pass

class LivePlot (threading.Thread):
    def __init__(self, threadID, name):
        threading.Thread.__init__(self)
        self.threadID = threadID
        self.name = name
        self.dead = False
        self.fileName = "ADC_Voltage.txt"
        self.fig = plt.figure()
        self.ax1 = self.fig.add_subplot(1,1,1)
        self.lastLine = 0
        self.xs = []
        self.ys = []

    def run(self):
        self.animate()
        plt.show()

    def animate(self):
        while not self.dead:
            fileFlag = False
            while not fileFlag:
                try:
                    graph_data = open("ADC_Voltage.txt", 'r').read()
                    fileFlag = True
                except:
                    continue
            lines = graph_data.split('\n')
            counter = self.lastLine
            while counter < len(lines)-1:
                if len(lines[counter]) > 1:
                    x,y = lines[counter].split(',')
                    tmp = int(x)/1250
                    self.xs.append(str(tmp))
                    self.ys.append(y)
                counter = counter + 1
            self.lastLine = counter
            self.ax1.clear()
            self.ax1.plot(self.xs, self.ys)
            time.sleep(0.2)
#https://code.i-harness.com/en/q/12c083a