#include "include/PaddleInf.h"

//inline tuple<size_t, size_t, size_t> getDim(Mat &src) { return {src.channels(), src.cols, src.rows}; }

/**
 * @brief 对外开放的推理接口
 * 
 * @param image 
 */
void PaddleInf::inference(cv::Mat image)
{
    preprocess(image);
    doInference();
    processOutput();
}

/**
 * @brief 初始化推理引擎
 * 
 * @param filename 
 */

void PaddleInf::setUpSession(std::string filename)
{
    paddle_infer::Config infer_config;
    //加载模型
    infer_config.SetModel(filename + "/model.pdmodel",
                          filename + "/model.pdiparams");
    
    //开启IR优化
    infer_config.SwitchIrOptim(true);
    //开启内存优化
    infer_config.EnableMemoryOptim();
    //以下三行是开启GPU推理用的
    // infer_config.DisableGpu();
    //infer_config.EnableUseGpu(1000, 0);
    //infer_config.EnableTensorRtEngine(1 << 30, 1, 10, paddle::AnalysisConfig::Precision::kFloat32, false, false);

    //设置cpu推理处理的线程 10
    infer_config.SetCpuMathLibraryNumThreads(10);
    //构建引擎
    predictor = paddle_infer::CreatePredictor(infer_config);
}

/**
 * @brief 转变图像通道为NCHW
 * 
 * @param hwc_img 
 * @param data 
 */
void PaddleInf::hwc_img_2_chw_data(const cv::Mat &hwc_img, float *data)
{
    int rows = hwc_img.rows;
    int cols = hwc_img.cols;
    int chs = hwc_img.channels();
    for (int i = 0; i < chs; ++i)
    {
        cv::extractChannel(hwc_img, cv::Mat(rows, cols, CV_32FC1, data + i * rows * cols), i);
    }
}

/**
 * @brief 图像简单预处理，包含resize和normalization 其中动态图模型不需要resize，直接送进去就好
 * 
 * @param img 
 * @return cv::Mat 
 */
cv::Mat PaddleInf::read_process_image(Mat img)
{
    cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
    //cv::resize(img, img, cv::Size(1024, 512));
    img.convertTo(img, CV_32F, 1.0 / 255, 0);
    img = (img - 0.5) / 0.5;
    return img;
}

/**
 * @brief 图像预处理全过程
 * 
 * @param img 
 */
void PaddleInf::preprocess(cv::Mat &img)
{
    //送进去normalize
    cv::Mat inp_img = read_process_image(img);
    int rows = img.rows;
    int cols = img.cols;
    int chs = img.channels();

    //准备将Mat格式转换为paddle引擎的输入
    std::vector<float> input_data(1 * chs * rows * cols, 0.0f);
    //调整通道顺序
    hwc_img_2_chw_data(inp_img, input_data.data());

    //获取引擎输入信息
    auto input_names = predictor->GetInputNames();
    auto input_t = predictor->GetInputHandle(input_names[0]);
    //调整推理引擎输入shape
    std::vector<int> input_shape;
    input_shape = {1, chs, rows, cols};
    input_t->Reshape(input_shape);
    //将准备好的input拷贝到内存准备推理
    input_t->CopyFromCpu(input_data.data());
}
/**
 * @brief 跑推理
 * 
 */
void PaddleInf::doInference()
{
    predictor->Run();
}

/**
 * @brief 对输出进行后处理，得到一个mask
 * 
 */
void PaddleInf::processOutput()
{
    //获取output信息
    auto output_names = predictor->GetOutputNames();
    auto output_t = predictor->GetOutputHandle(output_names[0]);
    std::vector<int> output_shape = output_t->shape();
    int out_num = std::accumulate(output_shape.begin(), output_shape.end(), 1,
                                  std::multiplies<int>());
    //将output信息拷贝出来
    std::vector<int64_t> out_data(out_num);
    output_t->CopyToCpu(out_data.data());

    //将output的信息转化为uint8的vector编码，再转换到Mat
    // Get pseudo image
    std::vector<uint8_t> out_data_u8(out_num);
    for (int i = 0; i < out_num; i++)
    {
        out_data_u8[i] = static_cast<uint8_t>(out_data[i]);
    }
    cv::Mat out_gray_img(output_shape[1], output_shape[2], CV_8UC1, out_data_u8.data());
    //拷贝出去
    out_gray_img.copyTo(result);

    //后期加入其他inference种类
    /*switch (INF_TYPE)
    {
    case (INFERENCE_TYPE::CLASSIFICATION):
    {
        output_blob = infer_request.GetBlob(output_name);
        ClassificationResult classificationResult(output_blob, 1, NUM_CLASSES);
        vector<unsigned> cresult = classificationResult.getResults();
        return cresult;
    }
    case (INFERENCE_TYPE::SEGMENTATION):
    {
        output_blob = infer_request.GetBlob(output_name);
    }
    }*/
}