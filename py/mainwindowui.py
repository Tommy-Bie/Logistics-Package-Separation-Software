# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'mainwindow.ui'
#
# Created by: PyQt5 UI code generator 5.9.2
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1843, 1080)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(MainWindow.sizePolicy().hasHeightForWidth())
        MainWindow.setSizePolicy(sizePolicy)
        MainWindow.setAutoFillBackground(True)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.Frame = QtWidgets.QLabel(self.centralwidget)
        self.Frame.setGeometry(QtCore.QRect(10, 10, 1711, 991))
        self.Frame.setObjectName("Frame")
        self.Show = QtWidgets.QPushButton(self.centralwidget)
        self.Show.setEnabled(True)
        self.Show.setGeometry(QtCore.QRect(1740, 30, 81, 50))
        self.Show.setAutoDefault(True)
        self.Show.setObjectName("Show")
        self.Segment = QtWidgets.QPushButton(self.centralwidget)
        self.Segment.setGeometry(QtCore.QRect(1740, 100, 81, 50))
        self.Segment.setObjectName("Segment")
        self.Get_ROI = QtWidgets.QPushButton(self.centralwidget)
        self.Get_ROI.setGeometry(QtCore.QRect(1740, 170, 81, 50))
        self.Get_ROI.setObjectName("Get_ROI")
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 1843, 22))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.Frame.setText(_translate("MainWindow", "Map"))
        self.Show.setText(_translate("MainWindow", "Show"))
        self.Segment.setText(_translate("MainWindow", "Segment"))
        self.Get_ROI.setText(_translate("MainWindow", "Get ROI"))

