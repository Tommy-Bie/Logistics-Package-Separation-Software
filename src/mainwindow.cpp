/**
 * @file mainwindow.cpp
 * @author Yi Qin (hotfox2001@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "mainwindow.h"
#include "./ui_mainwindow.h"

/**
 * @brief 这部分裸着，不好，要改掉
 * 
 */
Mat showroi;
Rect roiRect;
vector<Point> choosePoint;
int clickTimes;
bool isFinish;
void selectROI();
void mouseCallback(int event, int x, int y, int flags, void *);
RegParam param;

/**
 * @brief 将Opencv的Mat转换为QT中可以显示的QImage
 * 
 * @param mat 
 * @return QImage 
 */
QImage cvMat_to_QImage(const cv::Mat &mat)
{
    switch (mat.type())
    {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB32);
        return image;
    }

    // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image((const unsigned char *)(mat.data), mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }

    // 8-bit, 1 channel
    case CV_8UC1:
    {
        static QVector<QRgb> sColorTable;
        // only create our color table once
        if (sColorTable.isEmpty())
        {
            for (int i = 0; i < 256; ++i)
                sColorTable.push_back(qRgb(i, i, i));
        }
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        image.setColorTable(sColorTable);
        return image;
    }

    default:
        qDebug("Image format is not supported: depth=%d and %d channels\n", mat.depth(), mat.channels());
        break;
    }
    return QImage();
}

/**
 * @brief Construct a new Main Window:: Main Window object
 * 
 * @param parent 
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //初始化相机
    //Cam.init();

    //构建一个信号与槽，来完成图像与结果的动态刷新
    QSignalMapper *sm = new QSignalMapper(this);
    timerSHOW = new QTimer(this);
    connect(timerSHOW, SIGNAL(timeout()), sm, SLOT(map()));
    sm->setMapping(timerSHOW, 1);
    connect(sm, SIGNAL(mapped(int)), this, SLOT(display(int)));
}

/**
 * @brief Destroy the Main Window:: Main Window object
 * 
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief 在控件中显示图像
 * 
 * @param index 
 */
void MainWindow::display(int index)
{
    //获取图像
    /*Cam.getData();
    frame = Cam.getFrame();
    depth = Cam.getDepth();*/
    switch (index)
    {
    case 1:
    {
        if (isSeg)//开始分割了就送入对应的处理
            Point2d front = packseg.PackSegment(frame, depth, roiRect, _show);
        else//否则就直接显示现在的frame
            frame.copyTo(_show);
        cv::resize(_show, _show, Size(1711, 991));

        //转换并显示
        QImage img;
        img = cvMat_to_QImage(_show);
        ui->Frame->setPixmap(QPixmap::fromImage(img));
        ui->Frame->show();
        break;
    }
    }
}

/**
 * @brief 鼠标点选ROI的回调函数，用于选取ROI
 * 
 * @param event 
 * @param x 
 * @param y 
 * @param flags 
 */
void mouseCallback(int event, int x, int y, int flags, void *)
{
    if (event == EVENT_LBUTTONDOWN)//监听鼠标左键按下这一事件
    {
        //获取光标的坐标并存进去
        Point tmp = Point(x, y);
        choosePoint.emplace_back(tmp);                         //将点击点坐标放入容器
        circle(showroi, Point(x, y), 3, Scalar(0, 0, 255), 3); //画出点击点坐标
        imshow("SelectROI", showroi);                          //显示相机视角图片
        clickTimes += 1;
        if (clickTimes == 1)
            isFinish = true;
    }
}

/**
 * @brief 窗体中点击Get_ROI这一个按键的callback
 * 
 */
void MainWindow::on_Get_ROI_clicked()
{
    //显示摄像头画面，并在摄像头画面上选ROI进行检测
    frame.copyTo(showroi);
    imshow("SelectROI", showroi);
    waitKey(10);
    clickTimes = 0;
    isFinish = false;
    setMouseCallback("SelectROI", mouseCallback); //回调函数

    //等待框选完成
    while (true)
    {
        if (isFinish)
        {
            roiRect = Rect(choosePoint[0], choosePoint[1]);
            rectangle(showroi, Rect(choosePoint[0], choosePoint[1]), Scalar(255, 0, 0), 2);
            imshow("SelectROI", showroi);
            waitKey(1000);
            cv::FileStorage fs("ROISelected.yml", cv::FileStorage::WRITE);
            fs << "ROIRect" << roiRect;
            fs.release();
            cout << "Writing finished" << '\n';
            break;
        }
    }
}

/**
 * @brief 转换开始分割的flag
 * 
 */
void MainWindow::on_Segment_clicked()
{
    isSeg == true ? isSeg = false : isSeg = true;
}

/**
 * @brief 加载并展示画面
 * 
 */
void MainWindow::on_Show_clicked()
{
    ui->Frame->setScaledContents(true);
    timerSHOW->start(30);
}
