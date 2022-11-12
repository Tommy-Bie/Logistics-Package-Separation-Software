# Author: Eric Tan
# Creation Date: 2022/02/12
# ---------------------------
import sys
sys.path.append("./PaddleInference")
from PyQt5 import QtWidgets
from PyQt5.QtGui import QImage, QPixmap
from PyQt5.QtCore import QTimer
from mainwindowui import Ui_MainWindow
from PackDetect import PackageSegment, Point
import cv2
from messagebox import *

# 封装Rect类，用于roiRect
class my_Rect(object):
    def __init__(self, tl, br):
        self.x = tl.x
        self.y = tl.y
        self.w = br.x - tl.x
        self.h = br.y - tl.y

class MainWindow(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        self.setupUi(self)
        self.initUI()
    def initUI(self):
        # 常量定义
        self.isSeg = False
        self.cap = cv2.VideoCapture(0)
        self.timer_camera = QTimer()
        self.packseg = PackageSegment()
        self.choosePoint = []
        self.frame = 0
        self.roiRect = 0

        # 槽函数绑定
        self.Show.clicked.connect(self.Show_func)
        self.Segment.clicked.connect(self.Segment_func)
        self.Get_ROI.clicked.connect(self.Get_ROI_func)

        # 初始化相机
        # self.Cam = TYCamera()
        # self.Cam.init()

    # todo:timout信号连接，定时调用
    def display(self):
        # 获取图像
        # self.Cam.getData();
        # self.frame = self.Cam.getFrame()
        # self.depth = self.Cam.getDepth()

        # for test cap
        # ret, self.frame = self.cap.read()
        # self.frame = cv2.cvtColor(self.frame, cv2.COLOR_BGR2RGB)

        # for test
        self.frame = cv2.imread("38.jpg")
        # self.frame = cv2.cvtColor(self.frame, cv2.COLOR_BGR2RGB)

        if self.isSeg:  # 开始分割了就送入对应的处理，找到最前面包裹的中心点     # todo:传参show到底传不传？
            # front = self.packseg.PackSegment(self.frame, self.depth, self.roiRect, self.show)
            front = self.packseg.PackSegment(self.frame, self.depth, self.roiRect)
            print("front point:", front)
        else:   # 否则就直接显示现在的frame
            self.show = self.frame.copy()    # 或许应该这么写

        # 转换为QPixmap并显示
        # Opencv-->QImage
        qimage = QImage(self.show.data, self.show.shape[1], self.show.shape[0], QImage.Format_RGB888)
        # QImage-->QPixmap
        piximage = QPixmap.fromImage(qimage)
        self.Frame.setPixmap(piximage)   # label控件显示QPixmap图片对象
        self.Frame.setScaledContents(True)  # 让图片自适应label大小

    # Get_ROI按钮的槽函数
    def Get_ROI_func(self):
        # pass
        self.showroi = self.frame.copy()

        # 鼠标点选ROI的回调函数，用于选取ROI
        def mouseCallback(event, x, y, flags, param):
            if event == cv2.EVENT_LBUTTONDBLCLK:
                tmp = Point(x, y)
                self.choosePoint.append(tmp)

                xy = "%d,%d" % (x, y)
                cv2.circle(self.showroi, (x, y), 3, (0, 0, 255), 3)
                cv2.putText(self.showroi, xy, (x, y), cv2.FONT_HERSHEY_PLAIN,
                            1.0, (0, 0, 0), thickness=1)
                self.clickTimes += 1
                if self.clickTimes == 1:
                    self.isFinish = True

        cv2.namedWindow('SelectROI')
        cv2.setMouseCallback("SelectROI", mouseCallback)
        self.clickTimes = 0
        self.isFinish = False
        while True:
            cv2.imshow("SelectROI", self.showroi)
            if cv2.waitKey(1) == 27:
                cv2.destroyAllWindows()
                break

        # while True:
        if self.isFinish:
            tl = (self.choosePoint[0].x, self.choosePoint[0].y)     # topleft：左上角
            br = (self.choosePoint[1].x, self.choosePoint[1].y)     # belowright：右下角
            cv2.rectangle(self.showroi, tl, br, (255, 0, 0), 2)
            cv2.imshow("SelectROI", self.showroi)
            cv2.waitKey(1000)
            cv2.destroyAllWindows()

            self.roiRect = my_Rect(self.choosePoint[0], self.choosePoint[1])
            # 清空choosePoint
            self.choosePoint = []
            # 将ROI保存至yml文件，暂时不要
            # fs = cv2.FileStorage('abc.yml', cv2.FileStorage_WRITE)
            # fs.write('ROIRect', self.roiRect)
            # fs.release()
        else:
            messagebox3()

        print("Without writing, finished")

    # 转换开始分割的flag
    def Segment_func(self):
        try:
            if not self.roiRect:
                raise Exception("未选择ROI")
            self.isSeg = False if self.isSeg == True else True

            front = self.packseg.PackSegment(self.frame, self.frame, self.roiRect)
            print(f"front point:({front.x}, {front.y})")
        except:
            messagebox1()

    # 加载并展示画面
    def Show_func(self):
        self.Frame.setScaledContents(True)
        self.display()

        # self.camera_control()
        # # 从此开始刷新画面
        # self.timer_camera.timeout.connect(self.display)

    def camera_control(self):
        if self.timer_camera.isActive() == False:
            # 开始计时（开始展示）
            self.timer_camera.start(30)
        else:
            # 停止计时，关闭摄像头
            self.timer_camera.stop()
            self.cap.release()
