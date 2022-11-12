#include <algorithm>
#include <chrono>
#include <iostream>
#include <fstream>
#include <numeric>
#include "include/PaddleInf.h"

typedef struct YamlConfig
{
    std::string model_file;
    std::string params_file;
    bool is_normalize;
    bool is_resize;
    int resize_width;
    int resize_height;
} YamlConfig;

std::shared_ptr<paddle_infer::Predictor> create_predictor()
{
    paddle_infer::Config infer_config;
    infer_config.SetModel("/home/eason/PaddleInf/model.pdmodel",
                          "/home/eason/PaddleInf/model.pdiparams");
    infer_config.EnableMemoryOptim();

    auto predictor = paddle_infer::CreatePredictor(infer_config);
    return predictor;
}

void hwc_img_2_chw_data(const cv::Mat &hwc_img, float *data)
{
    int rows = hwc_img.rows;
    int cols = hwc_img.cols;
    int chs = hwc_img.channels();
    for (int i = 0; i < chs; ++i)
    {
        cv::extractChannel(hwc_img, cv::Mat(rows, cols, CV_32FC1, data + i * rows * cols), i);
    }
}

cv::Mat read_process_image(std::string filepath)
{
    cv::Mat img = cv::imread(filepath, cv::IMREAD_COLOR);
    cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
    //cv::resize(img, img, cv::Size(1024, 512));
    img.convertTo(img, CV_32F, 1.0 / 255, 0);
    img = (img - 0.5) / 0.5;
    return img;
}

int main(int argc, char *argv[])
{
    PaddleInf inf(1024, 512, 6, INFERENCE_TYPE::SEGMENTATION, "/home/eason/PaddleInf");
    VideoCapture cap;
    cap.open("/home/eason/PaddleInf/video.avi");
    while (1)
    {
        Mat image;
        Mat result;
        cap.read(image);
        imshow("1",image);
        waitKey(10);
        cv::TickMeter tick;
        tick.reset();
        tick.start();
        inf.inference(image);
        tick.stop();
        std::cout << tick.getTimeMilli() << endl;
        Mat _result;
        image.copyTo(_result,inf.result);
        imshow("2",_result);
        waitKey(10);

    }
}