# Author: Eric Tan
# Creation Date: 2022/02/12
# ---------------------------
import sys
from PyQt5 import QtWidgets
from mainwindow import MainWindow


if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    mainWindow = MainWindow()
    mainWindow.show()
    sys.exit(app.exec_())
