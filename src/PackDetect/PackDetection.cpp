#include "PackDetection.h"


/* Rect CoordinateCorrect(Rect &box, Rect roi)
{
    Rect tmp;
    tmp.x = box.x + roi.x;
    tmp.y = box.y + roi.y;
    return tmp;
} */

/**
 * @brief MOG进行前景分割的函数
 * 
 * @param frame 
 * @return Mat 
 */
Mat PackageSegment::MOGProcess(Mat &frame)
{
    //中值模糊降噪
    medianBlur(frame, frame, 7);
    //准备送入GPU的Mat容器
    GpuMat d_frame(frame);
    GpuMat d_fgmask;
    GpuMat d_fgimg;
    GpuMat d_bgimg;

    Mat fgmask;//mask
    Mat fgimg;//前景
    Mat fgimg1;
    Mat bgimg;//背景

    //使用CUDA加速MOG前景分离的过程
    param.mog->apply(d_frame, d_fgmask, 0.01);
    //将GPU的Mat容器内内容加载回CPU上
    d_fgimg.create(d_frame.size(), d_frame.type());
    d_fgimg.setTo(Scalar::all(0));
    d_frame.copyTo(d_fgimg, d_fgmask);
    d_fgmask.download(fgmask);
    d_fgimg.download(fgimg);

    //直接加mask
    fgimg.copyTo(fgimg1, fgmask);

    //imshow("foreground mask", fgmask);
    //waitKey(10);
    //resize(frame, frame, Size(720, 540));
    //resize(fgmask, fgmask, Size(800, 600));
    imshow("foreground image", fgmask);
    waitKey(1);
    return fgmask;
}

/**
 * @brief 该函数是使用MOG进行分割的方法的整体流程的组合
 * 
 * @param frame 
 * @return Point 
 */

Point PackageSegment::MOGSeg(Mat &frame)
{
    vector<vector<Point>> contours;//存边缘
    vector<Rect> Rectangleresult;
    Point front;
    Mat thres;
    MOGProcess(frame).copyTo(thres);//将分割后得到的mask copy到thres
    threshold(thres, thres, 30, 255, 0);//对mask做二值化得单通道mask
    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
    morphologyEx(thres, thres, MORPH_OPEN, element);//对mask做开运算减少轮廓间的粘连
    findContours(thres, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);//找轮廓
    front = findFront(contours);//找最前面的那一个点
    return front;
}

/**
 * @brief 通过深度相机所找的深度图来判断包裹的轮廓
 * 
 * @param frame 
 * @param depth 
 * @param DectResult 
 * @return Point 
 */
Point PackageSegment::depthEdge(Mat &frame, Mat &depth, Mat &DectResult)
{
    //All in ROI
    vector<vector<Point>> contours;
    vector<Rect> Rectangleresult;
    Mat Firstframe;
    Point front = Point(0, 0);
    frame.copyTo(Firstframe);
    Mat grad;
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;

    /**
     * @brief 这一段原本想得是利用深度图的梯度来获得轮廓，但是因为IR相机的特点不是狠准
     *  简而言之，相邻两个区域内，如果深度差的不多，那就在同一个平面上，如果深度差很多，那梯度将会很大，梯度大的地方将可以提取轮廓
     */
    //spatialGradient(depth, grad_x, grad_y);
    /* Sobel(depth, grad_x, CV_16UC1, 1, 0, 3);
    Sobel(depth, grad_y, CV_16UC1, 0, 1, 3);
    convertScaleAbs(grad_x, abs_grad_x);
    convertScaleAbs(grad_y, abs_grad_y);
    addWeighted(abs_grad_x, 1, abs_grad_y, 1, 0, grad);
    threshold(grad, grad, 60, 255,THRESH_BINARY); */

    //直接对深度图做闭运算，减少图像中的空洞，然后提取轮廓
    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
    morphologyEx(depth, depth, MORPH_CLOSE, element);
    findContours(depth, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    drawContours(DectResult, contours, -1, Scalar(0, 0, 255), 1);
    imshow("11", depth);
    waitKey(10);

    //对轮廓提取外接矩形
    for (auto contour : contours)
    {
        RotatedRect tmp = minAreaRect(contour);
        if (tmp.size.height * tmp.size.width <= 500)
            continue;
        Rect tmprect = tmp.boundingRect();
        Rectangleresult.emplace_back(tmprect);
    }
    for (auto r : Rectangleresult)
    {
        rectangle(frame, r, Scalar(0, 0, 255), 1);
    }

    //front = findFront(contours);
    return front;
}
/**
 * @brief 找到在运输带最前面的那个点（测试验证时用 已弃用）
 * 
 * @param contours 
 * @return Point 
 */
Point PackageSegment::findFront(vector<vector<Point>> contours)
{
    Point front;
    vector<Rect> Rectangleresult;
    int fronty = 1000;

    //先提取轮廓
    for (auto contour : contours)
    {
        RotatedRect tmp = minAreaRect(contour);
        if (tmp.size.height * tmp.size.width <= 500)
            continue;
        Rect tmprect = tmp.boundingRect();
        tmprect.x += 153;
        tmprect.y += 1;
        Rectangleresult.emplace_back(tmprect);
    }
    //迭代法找最前的点
    for (auto r : Rectangleresult)
    {
        int centery = r.y + r.height * 0.5;
        if (centery < fronty)
            fronty = centery;
        front = Point(r.x + r.width * 0.5, centery);
    }
    return front;
}

/**
 * @brief 现在用的找运输带最前面的点的函数
 * 
 * @return Point 
 */
Point PackageSegment::findFrontPoint()
{
    Point front;
    int fronty = 1000;
    for (auto r : packageList)
    {
        int centery = r.center.y + r.bbox.height * 0.5;
        if (centery < fronty)
        {
            fronty = centery;
            trackingPack = r;
        }
    }
    return front;
}

/**
 * @brief 现在用的提取包裹轮廓的方法。作为实例分割结果的后处理
 * 
 * @param mask 
 */
void PackageSegment::findPackage(Mat &mask)
{
    vector<vector<Point>> contours;
    Point front;
    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
    morphologyEx(mask, mask, MORPH_CLOSE, element);
    findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    int fronty = 1000;
    for (auto contour : contours)
    {
        RotatedRect tmp = minAreaRect(contour);
        if (tmp.size.height * tmp.size.width <= 500)
            continue;
        Rect tmprect = tmp.boundingRect();
        tmprect.x += 153;
        tmprect.y += 1;
        this->packageList.emplace_back(Package(tmprect));
    }
}