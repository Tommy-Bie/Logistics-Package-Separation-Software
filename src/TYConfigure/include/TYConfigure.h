/**
 * @file TYConfigure.h
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
#include <vector>
#include <map>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <string>
#include "common.hpp"
#include <TYCoordinateMapper.h>
#include <TYApi.h>
#include <TYImageProc.h>
#include "../../ParallelProcess/include/ParallelProcess.h"
using namespace cv;
using namespace dnn;
using namespace std;
using namespace cv::cuda;

struct CallbackData {
  int             index;
  TY_DEV_HANDLE   hDevice;
  bool            needUndistort;

  TY_CAMERA_CALIB_INFO depth_calib;
  TY_CAMERA_CALIB_INFO color_calib;
};
extern CallbackData cb_data;

class TYCamera
{
public:
    std::string ID, IP;
    TY_INTERFACE_HANDLE hIface = NULL;
    TY_ISP_HANDLE hColorIspHandle = NULL;
    TY_DEV_HANDLE hDevice = NULL;
    int32_t color, ir, depth;
    cv::Mat _depth, _irl, _irr, _color;
    char *frameBuffer[2];
    void init();
    void getData();
    cv::Mat getDepth();
    cv::Mat getFrame();
    void closeCamera();
    void removeOutlier();
    static inline int parseFrame(const TY_FRAME_DATA &frame, cv::Mat *pDepth, cv::Mat *pLeftIR, cv::Mat *pRightIR, cv::Mat *pColor, TY_ISP_HANDLE color_isp_handle = NULL)
    {
        for (int i = 0; i < frame.validCount; i++)
        {
            if (frame.image[i].status != TY_STATUS_OK)
                continue;

            // get depth image
            if (pDepth && frame.image[i].componentID == TY_COMPONENT_DEPTH_CAM)
            {
                *pDepth = cv::Mat(frame.image[i].height, frame.image[i].width,CV_16U, frame.image[i].buffer).clone();
            }
            // get left ir image
            if (pLeftIR && frame.image[i].componentID == TY_COMPONENT_IR_CAM_LEFT)
            {
                if (frame.image[i].pixelFormat == TY_PIXEL_FORMAT_MONO16)
                {
                    cv::Mat ir(frame.image[i].height, frame.image[i].width, CV_16U, frame.image[i].buffer);
                    cv::cvtColor(ir, *pLeftIR, cv::COLOR_BayerBG2GRAY);
                }
                else
                {
                    *pLeftIR = cv::Mat(frame.image[i].height, frame.image[i].width, CV_8U, frame.image[i].buffer).clone();
                }
            }
            // get right ir image
            if (pRightIR && frame.image[i].componentID == TY_COMPONENT_IR_CAM_RIGHT)
            {
                *pRightIR = cv::Mat(frame.image[i].height, frame.image[i].width, CV_8U, frame.image[i].buffer).clone();
            }
            // get BGR
            if (pColor && frame.image[i].componentID == TY_COMPONENT_RGB_CAM)
            {
                if (frame.image[i].pixelFormat == TY_PIXEL_FORMAT_JPEG)
                {
                    std::vector<uchar> _v((uchar *)frame.image[i].buffer, (uchar *)frame.image[i].buffer + frame.image[i].size);
                    *pColor = cv::imdecode(_v, cv::IMREAD_COLOR);
                }
                if (frame.image[i].pixelFormat == TY_PIXEL_FORMAT_YVYU)
                {
                    cv::Mat yuv(frame.image[i].height, frame.image[i].width, CV_8UC2, frame.image[i].buffer);
                    cv::cvtColor(yuv, *pColor, cv::COLOR_YUV2BGR_YVYU);
                }
                else if (frame.image[i].pixelFormat == TY_PIXEL_FORMAT_YUYV)
                {
                    cv::Mat yuv(frame.image[i].height, frame.image[i].width, CV_8UC2, frame.image[i].buffer);
                    cv::cvtColor(yuv, *pColor, cv::COLOR_YUV2BGR_YUYV);
                }
                else if (frame.image[i].pixelFormat == TY_PIXEL_FORMAT_RGB)
                {
                    cv::Mat rgb(frame.image[i].height, frame.image[i].width, CV_8UC3, frame.image[i].buffer);
                    cv::cvtColor(rgb, *pColor, cv::COLOR_RGB2BGR);
                }
                else if (frame.image[i].pixelFormat == TY_PIXEL_FORMAT_BGR)
                {
                    *pColor = cv::Mat(frame.image[i].height, frame.image[i].width, CV_8UC3, frame.image[i].buffer);
                }
                else if (frame.image[i].pixelFormat == TY_PIXEL_FORMAT_BAYER8GB)
                {
                    if (!color_isp_handle)
                    {
                        cv::Mat raw(frame.image[i].height, frame.image[i].width, CV_8U, frame.image[i].buffer);
                        cv::cvtColor(raw, *pColor, cv::COLOR_BayerGB2BGR);
                    }
                    else
                    {
                        cv::Mat raw(frame.image[i].height, frame.image[i].width, CV_8U, frame.image[i].buffer);
                        TY_IMAGE_DATA _img = frame.image[i];
                        pColor->create(_img.height, _img.width, CV_8UC3);
                        int sz = _img.height * _img.width * 3;
                        TY_IMAGE_DATA out_buff = TYInitImageData(sz, pColor->data, _img.width, _img.height);
                        out_buff.pixelFormat = TY_PIXEL_FORMAT_BGR;
                        int res = TYISPProcessImage(color_isp_handle, &_img, &out_buff);
                        if (res != TY_STATUS_OK)
                        {
                            //fall back to  using opencv api
                            cv::Mat raw(frame.image[i].height, frame.image[i].width, CV_8U, frame.image[i].buffer);
                            cv::cvtColor(raw, *pColor, cv::COLOR_BayerGB2BGR);
                        }
                    }
                }
                else if (frame.image[i].pixelFormat == TY_PIXEL_FORMAT_MONO)
                {
                    cv::Mat gray(frame.image[i].height, frame.image[i].width, CV_8U, frame.image[i].buffer);
                    cv::cvtColor(gray, *pColor, cv::COLOR_GRAY2BGR);
                }
            }
        }

        return 0;
    };
};
