#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "opencv2/opencv.hpp"
#include "Parameter/include/Parameter.h"
//#include "TYConfigure/include/TYConfigure.h"
#include "PackDetect/include/PackDetection.h"
#include <QSignalMapper>
#include <QAction>
#include <QTimer>
using namespace cv;
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

/**
 * @brief QT老把式 一个总括的类
 * 
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    Mat frame, roiFrame, _show, depth;
    QTimer *timerSHOW;
    bool isSeg;
    //TYCamera Cam;
    PackageSegment packseg;
    ~MainWindow();

private slots:
    void on_Get_ROI_clicked();

    void on_Segment_clicked();

    void on_Show_clicked();

private:
    Ui::MainWindow *ui;
    void display(int);
};

#endif // MAINWINDOW_H
