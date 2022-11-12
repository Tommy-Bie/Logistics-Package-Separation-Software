# Author: Eric Tan
# Creation Date: 2022/02/12
# ---------------------------
import numpy as np
from paddle.inference import Config
from paddle.inference import create_predictor
import paddleseg.transforms as T

# 后处理常量
NMS_THRESH = 0.45
BBOX_CONF_THRESH = 0.3

"""
Paddleinference库的高层封装，模块化处理，底层的推理引擎和版本可随时更换
"""
class PaddleInf():
    def __init__(self, num_classes, inference_type, file):
        self.num_classes = num_classes      # 2
        self.inference_type = inference_type    # "segmentation"
        self.setUpSession(file)
        transforms = [T.Normalize()]
        self._transforms = T.Compose(transforms)

    def inference(self, image):
        self.preprocess(image)
        self.doInference()
        self.processOutput()

    def transforms(self, img):
        return self._transforms(img)
    def _preprocess(self, img):
        return self.transforms(img)[0]

    # 构建引擎
    def setUpSession(self, filename):
        # filename：模型文件的目录
        # 加载模型
        infer_config = Config(filename + "/model.pdmodel",
                              filename + "/model.pdiparams")

        # 开启IR优化
        infer_config.switch_ir_optim()
        # 开启内存优化
        infer_config.enable_memory_optim()
        # 开启GPU推理
        # infer_config.enable_use_gpu(500, 0)
        # infer_config.enable_tensorrt_engine(workspace_size=1 << 30, precision_mode=PrecisionType.Float32,max_batch_size=1, min_subgraph_size=5, use_static=False, use_calib_mode=False)

        # 设置cpu推理处理的线程 10
        infer_config.set_cpu_math_library_num_threads(10)

        # 构建推理引擎
        self.predictor = create_predictor(infer_config)

    # 图像预处理全过程
    def preprocess(self, img):
        data = np.array([self._preprocess(img)])
        # 设置输入
        # 获取引擎输入信息
        input_names = self.predictor.get_input_names()
        input_tensor = self.predictor.get_input_handle(input_names[0])
        # 调整推理引擎输入shape
        input_tensor.reshape(data.shape)
        # 将准备好的input拷贝到内存，准备推理
        input_tensor.copy_from_cpu(data.copy())

    # Run推理
    def doInference(self):
        self.predictor.run()

    # 对输出进行后处理，得到一个mask
    def processOutput(self):
        # 获取output信息
        output_names = self.predictor.get_output_names()
        output_tensor = self.predictor.get_output_handle(output_names[0])
        # 将output信息拷贝出来
        output_data = output_tensor.copy_to_cpu()
        # print("Predict class index: ", np.argmax(output_data))

        # 将output信息转换为uint8编码
        out_gray_img = np.array(output_data).astype(np.uint8)
        self.result = out_gray_img.copy()



    # 已弃用：
    # # 转变图像通道-->NCHW
    # def hwc_img_2_chw_data(self, hwc_img):
    #     rows = hwc_img.shape[0]
    #     cols = hwc_img.shape[1]
    #     chs = hwc_img.shape[2]
    #
    #     # HWC --> CHW
    #     hwc_img = hwc_img.transpose((2,0,1))
    #     # CHW-->NCHW
    #     hwc_img = hwc_img.reshape(-1, chs, rows, cols)
    #     print("hwc_img shape:", hwc_img.shape)
    #     return hwc_img
    #
    # # 图像简单预处理：resize和normalizaiton；其中动态图模型不需要resize，直接送进去即可
    # def read_process_image(self, img):
    #     img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    #
    #     # 归一化至0-1
    #     img = img.astype(np.float32)
    #     img = img * 1.0 /255
    #     # 平移 + 缩放
    #     img = (img - 0.5) / 0.5
    #     return img








