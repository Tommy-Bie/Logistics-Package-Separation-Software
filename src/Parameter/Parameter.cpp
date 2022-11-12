#include "include/Parameter.h"

RegParam::RegParam()
{
    file = "/home/eason/easoncv/PackSeg/Loadings/points.yaml";
}
void loadroi(cv::String file)
{
    FileStorage fs(file, FileStorage::READ);
    fs["tl"] >> param.tl;
    fs["br"] >> param.br;
    fs.release();
}