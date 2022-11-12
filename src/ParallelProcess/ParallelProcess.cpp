#include "include/ParallelProcess.h"
#include <iostream>

// ---------------- parallel_for_ 并行运算处理 -----------------
/**
 * @brief 二值化循环处理 -- 构造函数
 * 
 * @param srcImg 源图像
 * @param binImg 二值图像
 */
ParallelRemoveOutlier::ParallelRemoveOutlier(Mat &srcImg, float _maxdepth)
{
    this->pSrcImg = &srcImg;
    this->maxdepth = _maxdepth;
}

/**
 * @brief 二值化循环处理 -- 重载()运算符
 * 
 * @param range 循环范围
 */
void ParallelRemoveOutlier::operator()(const Range &range) const
{
    Mat &src = *pSrcImg;
    for (int col = range.start; col < range.end; col++)
    {
        uchar *data_src = src.ptr<uchar>(col);
        for (int row = 0; row < this->pSrcImg->rows; row++)
        {
            if (data_src[row * this->pSrcImg->cols] == 0)
            {
                data_src[row * this->pSrcImg->cols] = 999;
            }
        };
    }
}