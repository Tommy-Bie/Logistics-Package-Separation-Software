
#include "include/TYConfigure.h"
CallbackData cb_data;
//注意sl::和cv::同名的混淆！！
void eventCallback(TY_EVENT_INFO *event_info, void *userdata)
{
    if (event_info->eventId == TY_EVENT_DEVICE_OFFLINE)
    {
        LOGD("=== Event Callback: Device Offline!");
        // Note:
        //     Please set TY_BOOL_KEEP_ALIVE_ONOFF feature to false if you need to debug with breakpoint!
    }
    else if (event_info->eventId == TY_EVENT_LICENSE_ERROR)
    {
        LOGD("=== Event Callback: License Error!");
    }
}

/**
 * @brief 初始化相机
 * 
 */
void TYCamera::init()
{
    //初始化library
    TYInitLib();

    //选择设备
    std::vector<TY_DEVICE_BASE_INFO> selected;
    selectDevice(TY_INTERFACE_ALL, ID, IP, 1, selected);
    TY_DEVICE_BASE_INFO &selectedDev = selected[0];

    //打开设备和界面
    TYOpenInterface(selectedDev.iface.id, &hIface);
    TYOpenDevice(hIface, selectedDev.id, &hDevice);
    int32_t allComps;
    
    //激活设备
    TYGetComponentIDs(hDevice, &allComps);
    TYSetEnum(hDevice, TY_COMPONENT_RGB_CAM, TY_ENUM_IMAGE_MODE, TY_IMAGE_MODE_RGB_640x480);
    TYEnableComponents(hDevice, TY_COMPONENT_RGB_CAM);

    //启用一个isp句柄来从bayer格式转rgb格式
    //create a isp handle to convert raw image(color bayer format) to rgb image
    TYISPCreate(&hColorIspHandle);
    //Init code can be modified in common.hpp
    //NOTE: Should set RGB image format & size before init ISP
    ColorIspInitSetting(hColorIspHandle, hDevice);

    //启动设备
    TYEnableComponents(hDevice, TY_COMPONENT_IR_CAM_LEFT);
    TYEnableComponents(hDevice, TY_COMPONENT_IR_CAM_RIGHT);
    int32_t image_mode;
    get_default_image_mode(hDevice, TY_COMPONENT_DEPTH_CAM, image_mode);
    TYSetEnum(hDevice, TY_COMPONENT_DEPTH_CAM, TY_ENUM_IMAGE_MODE, TY_IMAGE_MODE_DEPTH16_640x480);
    TYEnableComponents(hDevice, TY_COMPONENT_DEPTH_CAM);
    
    //从设备内存中获取图像到本机内存
    //depth map pixel format is uint16_t ,which default unit is  1 mm
    //the acutal depth (mm)= PixelValue * ScaleUnit
    float scale_unit = 1.;
    TYGetFloat(hDevice, TY_COMPONENT_DEPTH_CAM, TY_FLOAT_SCALE_UNIT, &scale_unit);
    uint32_t frameSize;
    TYGetFrameBufferSize(hDevice, &frameSize);
    frameBuffer[0] = new char[frameSize];
    frameBuffer[1] = new char[frameSize];
    TYEnqueueBuffer(hDevice, frameBuffer[0], frameSize);
    TYEnqueueBuffer(hDevice, frameBuffer[1], frameSize);
    TYRegisterEventCallback(hDevice, eventCallback, NULL);

    //获取相机参数
    TY_TRIGGER_PARAM trigger;
    trigger.mode = TY_TRIGGER_MODE_OFF;
    TYSetStruct(hDevice, TY_COMPONENT_DEVICE, TY_STRUCT_TRIGGER_PARAM, &trigger, sizeof(trigger));
    TYGetStruct(hDevice, TY_COMPONENT_DEPTH_CAM, TY_STRUCT_CAM_CALIB_DATA, &cb_data.depth_calib, sizeof(cb_data.depth_calib));
    TYGetStruct(hDevice, TY_COMPONENT_RGB_CAM, TY_STRUCT_CAM_CALIB_DATA, &cb_data.color_calib, sizeof(cb_data.color_calib));

    //开始工作
    TYStartCapture(hDevice);
}

/**
 * @brief 从设备中获取深度图和彩色图的data
 * 
 */
void TYCamera::getData()
{
    TY_FRAME_DATA frame;
    TYFetchFrame(hDevice, &frame, -1);
    parseFrame(frame, &_depth, &_irl, &_irr, &_color, hColorIspHandle);
    TYISPUpdateDevice(hColorIspHandle);
    TYEnqueueBuffer(hDevice, frame.userBuffer, frame.bufferSize);
}

/**
 * @brief 获取彩色图
 * 
 * @return cv::Mat 
 */
cv::Mat TYCamera::getFrame()
{
    return _color;
}

/**
 * @brief 获取深度值并进行后处理
 * 
 * @return cv::Mat 
 */
cv::Mat TYCamera::getDepth()
{
    _depth.convertTo(_depth, CV_16U);
    //threshold(_depth,_depth,60,255,THRESH_BINARY_INV);
    removeOutlier();
    _depth.convertTo(_depth, CV_8UC1);
    return _depth;
}

/**
 * @brief 关闭相机
 * 
 */
void TYCamera::closeCamera()
{
    TYStopCapture(hDevice);
    TYCloseDevice(hDevice);
    TYCloseInterface(hIface);
    TYISPRelease(&hColorIspHandle);
    TYDeinitLib();
    delete frameBuffer[0];
    delete frameBuffer[1];
}

/**
 * @brief 去除深度图中深度与深度之间不同的地方的黑边并完成深度图与彩色图对齐
 * 思想是统计深度图中出现的最多的深度值，那一定是包裹传送带本体的深度值，用这个深度值去填充空白区域
 * 
 */
void TYCamera::removeOutlier()
{

    /* float md;
    Mat hist_mat;
    int histSize = 1500;
    float range[] = {0, 1500};
    const float *histRange = {range};
    bool uniform = true;
    bool accumulate = false;
    calcHist(&_depth, 1, 0, Mat(), hist_mat, 1, &histSize, &histRange, uniform, accumulate);
    imshow("111", hist_mat);
    waitKey(10);
    double maxVal = 0;
    minMaxLoc(hist_mat, 0, &maxVal, 0, 0);
    md = (float)maxVal;
    cout << maxVal << '\n'; */

    TYMapDepthImageToColorCoordinate(
        &cb_data.depth_calib,
        _depth.cols, _depth.rows, _depth.ptr<uint16_t>(),
        &cb_data.color_calib,
        _depth.cols, _depth.rows, _depth.ptr<uint16_t>());
    threshold(_depth, _depth, 990, 975, THRESH_BINARY_INV);
    /* int row = _depth.rows;
    int col = _depth.cols;
    for (int r = 0; r < row; r++)
    {
        for (int c = 0; c < col; c++)
        {
            if (_depth.at<uint16_t>(r, c) * 1. >= 990)
                _depth.at<uint16_t>(r, c) = 0;
        };
    } */
/*     uint16_t md =999;
    const size_t cols = _depth.cols;
    parallel_for_(Range(0, cols), ParallelRemoveOutlier(_depth, md)); */
}
