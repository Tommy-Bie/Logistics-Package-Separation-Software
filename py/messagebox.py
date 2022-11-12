# Author: Eric Tan
# Creation Date: 2022/02/16
# ---------------------------
from PyQt5.QtWidgets import QMessageBox

def messagebox1():
    msgBox = QMessageBox()
    msgBox.setWindowTitle('Warning')
    msgBox.setText("Please select ROI!")
    # 隐藏ok按钮
    msgBox.addButton(QMessageBox.Ok)
    msgBox.button(QMessageBox.Ok).hide()
    # 模态对话框
    msgBox.exec_()

def messagebox2():
    msgBox = QMessageBox()
    msgBox.setWindowTitle('Warning')
    msgBox.setText("Please show image!")
    # 隐藏ok按钮
    msgBox.addButton(QMessageBox.Ok)
    msgBox.button(QMessageBox.Ok).hide()
    # 模态对话框
    msgBox.exec_()

def messagebox3():
    msgBox = QMessageBox()
    msgBox.setWindowTitle('Warning')
    msgBox.setText("Please choose 2 points!")
    # 隐藏ok按钮
    msgBox.addButton(QMessageBox.Ok)
    msgBox.button(QMessageBox.Ok).hide()
    # 模态对话框
    msgBox.exec_()