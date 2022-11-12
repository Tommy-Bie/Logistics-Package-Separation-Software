/**
 * @file PackDetection.h
 * @author Yi Qin (hotfox2001@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core/utility.hpp"
#include "opencv2/cudabgsegm.hpp"
#include "../../Parameter/include/Parameter.h"
#include "../../PaddleInf/include/PaddleInf.h"
#include "../../Tracker/include/Tracker.h"
//#include "../../TYConfigure/include/TYConfigure.h"
#include <vector>
#include <map>
#include <stdio.h>
#include <string.h>
using namespace cv;
/**
 * @brief Package类为检测到的每一个包裹创建的类
 * 
 */
class Package
{
private:
    //返回包裹bbox的中心点
    inline Point2d pcenter(Rect2d point) { return Point2d(point.tl().x + (point.x / 2), point.tl().y + (point.y / 2)); };

public:
    Package(Rect2d _bbox) : bbox(_bbox) { this->center = pcenter(bbox); };
    Package(){};
    Rect2d bbox;
    Point2d center;
    bool isFront;//是否是最前面的那一个
};

/**
 * @brief PackageSegment类为包裹分割总的控制类
 * 
 */
class PackageSegment
{
public:
    //前几个函数可以先不管，是用MOG等方式去做前景分离的函数，是Previous Work
    Point MOGSeg(Mat &);
    Point depthEdge(Mat &, Mat &, Mat &);
    Mat MOGProcess(Mat &frame);
    //下面的就比较有用
    Point findFront(vector<vector<Point>> contours);
    Point PackSegment(Mat &, Mat &, Rect, Mat &);
    Point findFrontPoint();
    void findPackage(Mat &mask);
    //校正坐标函数，从roi中的坐标转换回原来的图像中的坐标保证能够正常显示
    inline Point CoordinateCorrect(Point &point, Rect roi)
    {
        Point tmp;
        tmp.x = point.x + roi.x;
        tmp.y = point.y + roi.y;
        return tmp;
    };
    vector<Package> packageList;    //检测到的包裹的vector
    PackTracker<cv::TrackerKCF> tracker;    //目标跟踪
    Package trackingPack;   //正在跟踪中的那一个包裹
    PaddleInf _pack_infer = PaddleInf(2, _INFERENCE_TYPE::SEGMENTATION, ".");   //实例分割使用的推理引擎
};
