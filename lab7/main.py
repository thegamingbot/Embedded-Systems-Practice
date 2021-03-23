import matplotlib.pyplot as plt
import serial
import sys
from PyQt5 import QtCore, QtWidgets


ser = serial.Serial("COM3", 9600)  # Open port with baud rate


def red():
    ser.write(bytes(b'r\r'))


def green():
    ser.write(bytes(b'g\r'))


def blue():
    ser.write(bytes(b'b\r'))


def yellow():
    ser.write(bytes(b'y\r'))


def magenta():
    ser.write(bytes(b'm\r'))


def cyan():
    ser.write(bytes(b'c\r'))


def white():
    ser.write(bytes(b'w\r'))


def begWrite():
    ser.write(bytes(b'v\r'))


y = []


def plot():
    begWrite()

    # global x,y
    plt.ion()
    fig, ax = plt.subplots()
    ax.set_ylim(0, 4)

    while True:
        line = ser.readline().decode("utf-8")
        num = 0.0
        try:
            num = float(line)
        except ValueError:
            num = 0.0

        ax.set_xlim(int(len(y)/20)*20, int(len(y)/20)*20+20)
        y.append(num)
        ax.plot(y, c='b')
        fig.canvas.draw()
        fig.canvas.flush_events()


def styleSheet(border, color, bgColor):
    return """QPushButton {
                border-style: outset;
                color: rgb(255, 255, 255);
                font: 17pt "Verdana";
                border: 2px solid %s;
                padding: 5px;
                border-radius: 3px;
                opacity: 200;
            }
            QPushButton:hover {
                color: %s;
                background-color: %s;
                border-style: inset;
            }
            QPushButton:pressed {
                background-color: %s;
                border-style: inset;
            }""" % (border, color, border, bgColor)


def setButton(button, form, sizePolicy, border, color, bgColor, order, function):
    button.setSizePolicy(sizePolicy)
    button.setMinimumSize(QtCore.QSize(0, 60))
    button.setAutoFillBackground(False)
    button.setStyleSheet(styleSheet(border, color, bgColor))
    button.setAutoDefault(True)
    button.clicked.connect(function)
    form.setWidget(order, QtWidgets.QFormLayout.SpanningRole, button)


class MainWindow(QtWidgets.QWidget):

    chatApp = QtCore.pyqtSignal(list)
    fileTransfer = QtCore.pyqtSignal(list)
    quiz = QtCore.pyqtSignal(list)

    def __init__(self):
        super().__init__()

        self.widget = QtWidgets.QWidget(self)
        self.red = QtWidgets.QPushButton(self.widget)
        self.green = QtWidgets.QPushButton(self.widget)
        self.blue = QtWidgets.QPushButton(self.widget)
        self.white = QtWidgets.QPushButton(self.widget)
        self.yellow = QtWidgets.QPushButton(self.widget)
        self.magenta = QtWidgets.QPushButton(self.widget)
        self.cyan = QtWidgets.QPushButton(self.widget)
        self.plot = QtWidgets.QPushButton(self.widget)

        self.formLayout_2 = QtWidgets.QFormLayout()
        self.verticalLayout_3 = QtWidgets.QVBoxLayout()
        self.verticalLayout_2 = QtWidgets.QVBoxLayout(self.widget)
        self.horizontalLayout_3 = QtWidgets.QHBoxLayout()
        self.verticalLayout = QtWidgets.QVBoxLayout(self)
        self.setupUI()

    def setupUI(self):
        self.formLayout_2.setContentsMargins(50, 35, 59, -1)

        spacerItem = QtWidgets.QSpacerItem(
            20, 40, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        self.verticalLayout_3.addItem(spacerItem)

        sizePolicy = QtWidgets.QSizePolicy(
            QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(
            self.red.sizePolicy().hasHeightForWidth())

        setButton(self.red, self.formLayout_2,
                  sizePolicy, "red", "white", "#800000", 0, red)
        setButton(self.blue, self.formLayout_2,
                  sizePolicy, "blue", "white", "#000080", 1, blue)
        setButton(self.green, self.formLayout_2,
                  sizePolicy, "green", "white", "#004000", 2, green)
        setButton(self.yellow, self.formLayout_2,
                  sizePolicy, "yellow", "black", "#808000", 3, yellow)
        setButton(self.magenta, self.formLayout_2,
                  sizePolicy, "magenta", "white", "#800080", 4, magenta)
        setButton(self.cyan, self.formLayout_2,
                  sizePolicy, "cyan", "black", "#008080", 5, cyan)
        setButton(self.white, self.formLayout_2,
                  sizePolicy, "white", "black", "#c0c0c0", 6, white)
        setButton(self.plot, self.formLayout_2,
                  sizePolicy, "orange", "black", "#ffc966", 7, plot)

        self.verticalLayout_3.addLayout(self.formLayout_2)

        spacerItem1 = QtWidgets.QSpacerItem(
            20, 40, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        self.verticalLayout_3.addItem(spacerItem1)

        self.verticalLayout.setContentsMargins(0, 0, 0, 0)

        self.widget.setMaximumSize(QtCore.QSize(16777215, 16777215))
        self.widget.setStyleSheet(".QWidget{background-color: rgb(0, 0, 0);}")

        self.verticalLayout_2.setContentsMargins(9, 0, 0, 0)

        self.verticalLayout_3.setContentsMargins(-1, 15, -1, -1)

        spacerItem1 = QtWidgets.QSpacerItem(
            20, 40, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        self.verticalLayout_3.addItem(spacerItem1)
        self.verticalLayout_2.addLayout(self.verticalLayout_3)

        self.horizontalLayout_3.addWidget(self.widget)
        self.horizontalLayout_3.setStretch(0, 1)
        self.verticalLayout.addLayout(self.horizontalLayout_3)

        self.retranslateUi()
        QtCore.QMetaObject.connectSlotsByName(self)

    def retranslateUi(self):
        _translate = QtCore.QCoreApplication.translate
        self.setWindowTitle(_translate("Form", "New"))
        self.red.setText(_translate("Form", "Red"))
        self.blue.setText(_translate("Form", "Blue"))
        self.green.setText(_translate("Form", "Green"))
        self.white.setText(_translate("Form", "White"))
        self.yellow.setText(_translate("Form", "Yellow"))
        self.magenta.setText(_translate("Form", "Magenta"))
        self.cyan.setText(_translate("Form", "Cyan"))
        self.plot.setText(_translate("Form", "Plot"))


if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())
