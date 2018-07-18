# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'SRG_GUI.ui'
#
# Created: Wed Jul 11 20:03:30 2018
#      by: PyQt4 UI code generator 4.10.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui
import serialRW as ser

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName(_fromUtf8("MainWindow"))
        MainWindow.resize(390, 404)
        self.centralwidget = QtGui.QWidget(MainWindow)
        self.centralwidget.setObjectName(_fromUtf8("centralwidget"))
        self.groupBox = QtGui.QGroupBox(self.centralwidget)
        self.groupBox.setGeometry(QtCore.QRect(10, 10, 371, 131))
        self.groupBox.setObjectName(_fromUtf8("groupBox"))
        self.serialPortList = QtGui.QComboBox(self.groupBox)
        self.serialPortList.setGeometry(QtCore.QRect(130, 30, 121, 27))
        self.serialPortList.setObjectName(_fromUtf8("serialPortList"))
        self.label = QtGui.QLabel(self.groupBox)
        self.label.setGeometry(QtCore.QRect(0, 30, 131, 21))
        self.label.setObjectName(_fromUtf8("label"))
        self.refreshButton = QtGui.QPushButton(self.groupBox)
        self.refreshButton.setGeometry(QtCore.QRect(260, 30, 98, 27))
        self.refreshButton.setObjectName(_fromUtf8("refreshButton"))
        self.label_2 = QtGui.QLabel(self.groupBox)
        self.label_2.setGeometry(QtCore.QRect(1, 60, 131, 21))
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.baudRateList = QtGui.QComboBox(self.groupBox)
        self.baudRateList.setGeometry(QtCore.QRect(130, 60, 121, 27))
        self.baudRateList.setObjectName(_fromUtf8("baudRateList"))
        self.connectButton = QtGui.QPushButton(self.groupBox)
        self.connectButton.setGeometry(QtCore.QRect(153, 90, 98, 27))
        self.connectButton.setObjectName(_fromUtf8("connectButton"))
        self.disconnectButton = QtGui.QPushButton(self.groupBox)
        self.disconnectButton.setGeometry(QtCore.QRect(260, 90, 98, 27))
        self.disconnectButton.setObjectName(_fromUtf8("disconnectButton"))
        self.groupBox_2 = QtGui.QGroupBox(self.centralwidget)
        self.groupBox_2.setGeometry(QtCore.QRect(10, 160, 371, 101))
        self.groupBox_2.setObjectName(_fromUtf8("groupBox_2"))
        self.label_3 = QtGui.QLabel(self.groupBox_2)
        self.label_3.setGeometry(QtCore.QRect(58, 31, 81, 17))
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.onAngleInput = QtGui.QLineEdit(self.groupBox_2)
        self.onAngleInput.setGeometry(QtCore.QRect(130, 27, 113, 27))
        self.onAngleInput.setObjectName(_fromUtf8("onAngleInput"))
        self.label_4 = QtGui.QLabel(self.groupBox_2)
        self.label_4.setGeometry(QtCore.QRect(53, 63, 81, 17))
        self.label_4.setObjectName(_fromUtf8("label_4"))
        self.onAngleInput_2 = QtGui.QLineEdit(self.groupBox_2)
        self.onAngleInput_2.setGeometry(QtCore.QRect(130, 59, 113, 27))
        self.onAngleInput_2.setObjectName(_fromUtf8("onAngleInput_2"))
        self.setAnglesButton = QtGui.QPushButton(self.groupBox_2)
        self.setAnglesButton.setGeometry(QtCore.QRect(260, 26, 98, 27))
        self.setAnglesButton.setObjectName(_fromUtf8("setAnglesButton"))
        self.getAnglesButton = QtGui.QPushButton(self.groupBox_2)
        self.getAnglesButton.setGeometry(QtCore.QRect(260, 59, 98, 27))
        self.getAnglesButton.setObjectName(_fromUtf8("getAnglesButton"))
        self.groupBox_3 = QtGui.QGroupBox(self.centralwidget)
        self.groupBox_3.setGeometry(QtCore.QRect(10, 280, 371, 80))
        self.groupBox_3.setObjectName(_fromUtf8("groupBox_3"))
        self.startAquisitionButton = QtGui.QPushButton(self.groupBox_3)
        self.startAquisitionButton.setGeometry(QtCore.QRect(49, 30, 121, 41))
        self.startAquisitionButton.setObjectName(_fromUtf8("startAquisitionButton"))
        self.stopAquisitionButton = QtGui.QPushButton(self.groupBox_3)
        self.stopAquisitionButton.setGeometry(QtCore.QRect(200, 30, 121, 41))
        self.stopAquisitionButton.setObjectName(_fromUtf8("stopAquisitionButton"))
        MainWindow.setCentralWidget(self.centralwidget)
        self.statusbar = QtGui.QStatusBar(MainWindow)
        self.statusbar.setObjectName(_fromUtf8("statusbar"))
        MainWindow.setStatusBar(self.statusbar)

        self.refreshButton.clicked.connect(self.refreshSerialPorts)
        self.connectButton.clicked.connect(self.connectToSerial)
        self.disconnectButton.clicked.connect(self.disconnectFromSerial)
        self.baudRateList.clear()
        baudRates = ["115200", "57600", "38400", "19200", "9600"]
        self.baudRateList.addItems(baudRates)

        self.getAnglesButton.clicked.connect(self.parseAngles)


        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(_translate("MainWindow", "SRG GUI", None))
        self.groupBox.setTitle(_translate("MainWindow", "Serial Port Configuration", None))
        self.label.setText(_translate("MainWindow", "Choose Serial Port:", None))
        self.refreshButton.setText(_translate("MainWindow", "Refresh List", None))
        self.label_2.setText(_translate("MainWindow", "Choose Baud Rate:", None))
        self.connectButton.setText(_translate("MainWindow", "Connect", None))
        self.disconnectButton.setText(_translate("MainWindow", "Disconnect", None))
        self.groupBox_2.setTitle(_translate("MainWindow", "Angle Configuration:", None))
        self.label_3.setText(_translate("MainWindow", "ON Angle:", None))
        self.label_4.setText(_translate("MainWindow", "OFF Angle:", None))
        self.setAnglesButton.setText(_translate("MainWindow", "Set Angles", None))
        self.getAnglesButton.setText(_translate("MainWindow", "Get Angles", None))
        self.groupBox_3.setTitle(_translate("MainWindow", "Aquisition:", None))
        self.startAquisitionButton.setText(_translate("MainWindow", "Start Aquisition", None))
        self.stopAquisitionButton.setText(_translate("MainWindow", "Stop Aquisition", None))

    def refreshSerialPorts(self):
        listOfPorts = s.listOfAvailableSerialPorts()
        self.serialPortList.clear()
        self.serialPortList.addItems(listOfPorts)
    
    def connectToSerial(self):
        s.serialPort = self.serialPortList.itemText(self.serialPortList.currentIndex())
        s.baudRate = self.baudRateList.itemText(self.baudRateList.currentIndex())
        if len(s.serialPort) > 0:
            s.openPort()
            """
            msgBox = QtGui.QMessageBox(self)
            msgBox.setIcon(QtGui.QMessageBox.Information)
            msgBox.setText("Connection sucessful")
            msgBox.setInformativeText("Do you want to reset target?")
            msgBox.addButton(QtGui.QMessageBox.Yes)
            msgBox.addButton(QtGui.QMessageBox.No)
            msgBox.setDefaultButton(QtGui.QMessageBox.Yes)
            ret = msgBox.exec_()
            if ret == QtGui.QMessageBox.Yes:
                s.sendString("!SR\r\n") #software reset of mcu
            else:
                return
            """
        else:
            print("noserial")

    def parseAngles(self):
        if s.isOpen():
            s.setSendString("!A?")
            angles = s.getMessage()
            onAngle = angles[3] + angles[4]
            offAngle = angles[6] + angles[7]
            self.onAngleInput.setText(onAngle)
            self.onAngleInput_2.setText(offAngle)


    def disconnectFromSerial(self):
        s.closePort()

if __name__ == "__main__":
    import sys
    s = ser.serialRW()
    app = QtGui.QApplication(sys.argv)
    MainWindow = QtGui.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    ui.refreshSerialPorts()
    MainWindow.show()
    sys.exit(app.exec_())

    """
    TODO:
-set-anje uglova 
startanje akvizicije
    """
    

