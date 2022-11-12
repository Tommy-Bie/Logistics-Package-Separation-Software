/**
 * @file Tracker.h
 * @author Yi Qin (hotfox2001@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <map>
#include <stdio.h>
#include <string.h>
#include <opencv2/tracking/tracker.hpp>
using namespace cv;

/**
 * @brief 调用opencv内置的六种图像跟踪算法对物流包裹进行跟踪的统一接口，与自己的KalmanFilter统一起来
 *        提供高层接口进行统一调用
 * 
 * @tparam T 
 */
template <class T>
class PackTracker
{
private:
    Ptr<T> TrackerPtr;
    inline Point2d pcenter(Rect2d point) { return Point2d(point.tl().x + (point.x / 2), point.tl().y + (point.y / 2)); };

public:
    PackTracker()
    {
        this->TrackerPtr = T::create();
    };

    double update(Mat &image, Rect2d &bbox)
    {
        this->TrackerPtr->update(image, bbox);
        return pcenter(bbox).x;
    };
};