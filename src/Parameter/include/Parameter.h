#ifndef PROCESS_H
#define PROCESS_H

#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core/utility.hpp"
#include "opencv2/cudabgsegm.hpp"
#include <vector>
#include <map>
#include <stdio.h>
#include <string.h>

using namespace cv;
using namespace dnn;
using namespace std;
using namespace cv::cuda;

struct RegParam
{
    Ptr<BackgroundSubtractor> mog = cuda::createBackgroundSubtractorMOG2();
    Point tl;
    Point br;
    cv::String file;
    RegParam();
};
extern RegParam param;
void loadroi(cv::String file);

#endif
