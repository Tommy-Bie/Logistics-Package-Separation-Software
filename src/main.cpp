#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <map>
#include <string>
#include "Parameter/include/Parameter.h"
//#include "TYConfigure/include/TYConfigure.h"
#include "PackDetect/include/PackDetection.h"
#include "mainwindow.h"
#include <QApplication>
#include <thread>


using namespace std;
using namespace cv;

/**
 * @brief main就是把控制权交给QT那边，勿念
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

/* inline void ratioresize(Mat &src, Mat &out, double ratio)
{
    double resizedwidth = src.cols * ratio;
    Size dim(resizedwidth, (src.rows * ratio));
    resize(src, out, dim);
}

Point ratiopoint(Point &src, double ratio)
{
    Point out;
    out.x = src.x * ratio;
    out.y = src.y * ratio;
    return out;
}

Mat norm_0_255(const Mat &src)
{
    Mat dst;
    switch (src.channels())
    {
    case 1:
        cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
        break;
    case 3:
        cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
        break;
    default:
        src.copyTo(dst);
        break;
    }
    return dst;
} */
