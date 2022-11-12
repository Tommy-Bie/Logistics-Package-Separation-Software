/**
 * @file ParallelProcess.h
 * @author Yi Qin (hotfox2001@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef PD
#define PD

#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core/utility.hpp"
#include "opencv2/cudabgsegm.hpp"
#include "../../Parameter/include/Parameter.h"
#include <vector>
#include <map>
#include <stdio.h>
#include <string.h>
using namespace cv;

// 二值化循环处理
class ParallelRemoveOutlier : public ParallelLoopBody
{
public:
    // 构造函数
    ParallelRemoveOutlier(Mat &srcImg,float _maxdepth);
    // 重载()运算符
    void operator()(const Range &range) const;


private:
    Mat *pSrcImg;
    float maxdepth;
    
};

#endif