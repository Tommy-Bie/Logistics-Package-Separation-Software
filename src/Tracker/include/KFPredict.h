/**
 * @file KFPredict.h
 * @author Yi Qin (hotfox2001@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui.hpp"
#include <stdio.h>
using namespace cv;

/**
 * @brief KFFilter为利用卡尔曼滤波对包裹轨迹进行预测的实现类
 * **正在构建中
 */
class KFTracker
{
private:
	KalmanFilter KF = KalmanFilter(2, 1, 0);
	Mat state = Mat(2, 1, CV_32F); /* (phi, delta_phi) */
	Mat processNoise = Mat(2, 1, CV_32F);
	Mat measurement = Mat::zeros(1, 1, CV_32F);

public:
	Ptr<KFTracker> create()
	{
		return this;
	}
	void init(Mat &image, Rect2d &bbox)
	{
		KF.transitionMatrix = (Mat_<float>(2, 2) << 1, 0, 0, 1);
		setIdentity(KF.measurementMatrix);
		setIdentity(KF.processNoiseCov, Scalar::all(1e-5));
		setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));
		setIdentity(KF.errorCovPost, Scalar::all(1));
		randn(KF.statePost, Scalar::all(0), Scalar::all(0.1));
	}
	void update(Mat &image,Rect2d &bbox){};
};