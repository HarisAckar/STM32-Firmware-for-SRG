import threading
import serialRW

class srgThread (threading.Thread):
    def __init__(self, threadID, name, serialRW):
        threading.Thread.__init__(self)
        self.threadID = threadID
        self.name = name
        self.serial = serialRW
        self.dead = False
        self.paused = False
    
    def run(self):
        self.readAquisition()
    
    def readAquisition(self):
        while not self.dead:
            if not self.paused:
                data = self.serial.getMessage()
                if data != None:
                    try:
                        value = float(data[1:])
                        value = value * 3 / 255
                        print(value)
                    except:
                        pass
    

