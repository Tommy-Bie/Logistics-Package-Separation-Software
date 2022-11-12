#include "include/PackDetection.h"
/**
 * @brief 包裹分割流程的主流程
 * 
 * @param frame 
 * @param depth 
 * @param roi 
 * @param SegResult 
 * @return Point 
 */
Point PackageSegment::PackSegment(Mat &frame, Mat &depth, Rect roi, Mat &SegResult)
{
    //先把检测区域画出来
    rectangle(frame, roi, Scalar(255, 0, 0), 2);
    //再把ROI切出来
    Mat rframe, rdepth;
    rframe = frame(roi);
    rdepth = depth(roi);
    //直接送ROI进去推理，目前是CPU推理
    _pack_infer.inference(rframe);
    //找包裹
    findPackage(_pack_infer.result);
    //找最前的点
    Point frontPoint = findFrontPoint();
    //还有跟踪，正在思考调整逻辑
    return frontPoint;
}
//point1 = MOGSeg(rframe);
//depthEdge(rframe, rdepth,SegResult);
//circle(frame, point1, 3, Scalar(0, 0, 255), -1);
//circle(frame, point2, 3, Scalar(0, 0, 255), -1);
//rectangle(frame, roi, Scalar(255, 0, 0), 2);
//resize(frame, frame, Size(800, 600));
//imshow("test", frame);
//imshow("test", rframe);
//waitKey(10);
