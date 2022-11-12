from pickletools import uint8
import time
import numpy as np
from onnxruntime import InferenceSession
import cv2
import numpy as np

# 加载ONNX模型
sess = InferenceSession('output.onnx')

image = cv2.imread('38.jpg')
image=cv2.resize(image,(1024,512))
cv2.normalize(image,image,0,255,cv2.NORM_MINMAX)
#print(image)
image=image.transpose((2,0,1))
img=np.array(image).astype('float32')
# 准备输入
inputname=sess.get_inputs()[0].name
outputname=sess.get_outputs()[0].name

# 模型预测
start = time.time()
ort_outs = sess.run(None, input_feed={inputname: img[None, :, :, :]})
pred = ort_outs[0].astype('uint8')
pred = pred.flatten()
print(pred)
#ret,thres = cv2.threshold(array,1,255,cv2.THRESH_BINARY)
cv2.imshow('t',pred)
cv2.waitKey()
end = time.time()
