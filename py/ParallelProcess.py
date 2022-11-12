# Author: Eric Tan
# Creation Date: 2022/02/13
# ---------------------------
import cv2
# import Parameter


class ParallelRemoveOutlier():
    def __init__(self, pImg, maxdepth):
        self.pImg = pImg
        self.maxdepth = maxdepth