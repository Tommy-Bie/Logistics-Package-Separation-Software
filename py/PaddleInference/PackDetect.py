# Author: Eric Tan
# Creation Date: 2022/02/12
# ---------------------------
import sys
sys.path.append("./")
import cv2
import numpy as np
from PaddleInf import PaddleInf
import matplotlib.pyplot as plt

# 封装Point类
class Point(object):
    def __init__(self, x, y):
        self.x = x
        self.y = y


"""
Package类为检测到的每一个包裹创建的类
"""
class Package():
    # bbox:Rect2d
    # center:Point2d
    def __init__(self, bbox):
        self.center = self.pcenter(bbox)

    def pcenter(self, bbox):
        return Point(bbox[0] + bbox[2]/2, bbox[1] + bbox[3]/2)


"""
PackageSegment类为包裹分割总的控制类
"""
class PackageSegment():
    def __init__(self):
        self.packageList = []
        # self.trackingPack  # 待跟踪的包裹已由下面算法确定
        # self.tracker   # 如何跟踪，还没确定

    # 包裹分割流程的主流程
    def PackSegment(self, frame, depth, roi):
        # 先画出检测区域
        x, y, w, h = roi.x, roi.y, roi.w, roi.h
        tl = (x, y)
        br = (x+w, y+h)
        cv2.rectangle(frame, tl, br, (255, 0, 0), 2)
        # 再把ROI切出来
        self.rframe = frame[y:y+h, x:x+w]
        while True:
            cv2.imshow("rframe", self.rframe)
            if cv2.waitKey(1) == 27:
                cv2.destroyAllWindows()
                break
        rdepth = depth[y:y+h, x:x+w]

        # 实例化推理引擎
        self.pack_infer = PaddleInf(2, "segmentation", "./models")
        # 直接送ROI进去做推理，目前是CPU推理
        self.pack_infer.inference(self.rframe)
        # 找包裹
        self.findPackage(self.pack_infer.result)
        # 找最前的点
        frontPoint = self.findFrontPoint()

        # todo:跟踪，正在思考调整逻辑

        # 清空包裹列表
        self.packageList = []

        return frontPoint

    # 现在用点的提取包裹轮廓的方法，作为实例分割结果的后处理
    def findPackage(self, mask):
        kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))
        mask = np.squeeze(mask)  # 去掉第一维(1, rows, cols)-->(rows, cols)
        seg_result = mask.copy()
        mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)   # 开运算去噪
        contours, hierarchy = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        cv2.drawContours(self.rframe, contours, -1, (255, 0, 0), 2)

        # 展示分割结果
        plt.subplot(131), plt.imshow(seg_result, cmap='gray'), plt.title("segformer result")
        plt.subplot(132), plt.imshow(mask, cmap='gray'), plt.title("morph_open result")
        plt.subplot(133), plt.imshow(self.rframe), plt.title("contours result")
        plt.show()

        for contour in contours:
            # 1.找到点集的RotatedRect
            tmp_rrect = cv2.minAreaRect(contour)
            # tmp: Box2D对象(tuple 3)： （中心点坐标，（宽度，高度）,旋转的角度）
            if tmp_rrect[1][0] * tmp_rrect[1][1] <= 500:
                continue

            # 2.找到点集的Rect
            tmp_rect = list(cv2.boundingRect(contour))    # tmp_rect: (x, y, w, h)
            tmp_rect[0] += 153
            tmp_rect[1] += 1

            self.packageList.append(Package(tmp_rect))  # Package类对象，其属性center为Point类

    # 现在用的找运输带最前面的点的函数
    def findFrontPoint(self):
        fronty = 1000
        # 迭代法找最前面的点
        for r in self.packageList:
            # r: Package对象，具有属性center； center是Point对象，为中心点坐标
            centery = r.center.y
            if centery <fronty:
                fronty = centery
                self.trackingPack = r       # 待跟踪的包裹
                front = Point(r.center.x, r.center.y)
        return front



    # # MOG进行前景分割的函数
    # def MOGProcess(self, frame):
    #     pass
    # # 通过深度相机所找的深度图来判断包裹的轮廓
    # def depthEdge(self, frame, depth, DectResult):
    #     pass

    # # 使用MOG分割的方法的整体流程组合
    # def MOGSeg(self, frame):
    #     # 将分割后得到的mask拷贝到thres
    #     thres = self.MOGProcess(frame)
    #     # 对mask做二值化得到单通道mask
    #     ret, thres = cv2.threshold(thres, 30, 255, 0)
    #     # 对mask做开运算减少轮廓件的粘连
    #     kernel = np.ones((5, 5),np.uint8)
    #     thres = cv2.morphologyEx(thres, cv2.MORPH_OPEN, kernel)
    #     # 找轮廓
    #     contours, hierarchy = cv2.findContours(thres, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    #     # 找最前面的那一个点
    #     front = self.findFront(contours)
    #     return front







