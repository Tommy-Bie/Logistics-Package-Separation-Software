/**
 * @file PaddleInf.h
 * @author Yi Qin (hotfox2001@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once
#include <iterator>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <opencv2/opencv.hpp>
#include "../paddle_inference/paddle/include/paddle_inference_api.h"

using namespace cv;

using namespace std;
#define NMS_THRESH 0.45
#define BBOX_CONF_THRESH 0.3

enum _INFERENCE_TYPE
{
    CLASSIFICATION,
    SEGMENTATION,
    DETECTTION
};

/**
 * @brief 这个类是Paddleinference库的高层封装，模块化处理，底层的推理引擎和版本可随时更换
 * 
 */
class PaddleInf
{
private:
    std::shared_ptr<paddle_infer::Predictor> predictor;
    void hwc_img_2_chw_data(const cv::Mat &hwc_img, float *data);
    cv::Mat read_process_image(Mat img);
    void preprocess(Mat&);
    void doInference();
    void processOutput();
    void setUpSession(string);
    int _NUM_CLASSES;
    int _INF_TYPE;

public:
    PaddleInf( int _num_classes, int _INF_TYPE, string file) : _NUM_CLASSES(_num_classes), _INF_TYPE(_INF_TYPE)
    {
        setUpSession(file);
    };                                         //初始化
    cv::Mat result;
    void inference(cv::Mat image); //推理，结果装在result里
};