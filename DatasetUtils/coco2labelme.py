# -*- coding: utf-8 -*-
"""
@author: Taoting
将用coco格式的json转化成labeime标注格式的json
"""

import json
import cv2
import numpy as np
import os
   
#用一个labelme格式的json作为参考，因为很多信息都是相同的，不需要修改。
def reference_labelme_json():
    ref_json_path = './bin/25.json'
    data=json.load(open(ref_json_path))    
    return data
    
def labelme_shapes(data,data_ref,id):
    shapes = []
    label_num = {'box':0}#根据你的数据来修改
    for ann in data['annotations']:
        if id == ann['image_id']:
            shape = {}
            class_name = [i['name'] for i in data['categories'] if i['id'] == ann['category_id']]
            #label要对应每一类从_1开始编号
            label_num[class_name[0]] += 1
            shape['label'] = class_name[0]

            shape['points'] = []
            # ~ print(ann['segmentation'])
            if not type(ann['segmentation']) == list:
                continue
            else:
                x = ann['segmentation'][0][::2]#奇数个是x的坐标
                y = ann['segmentation'][0][1::2]#偶数个是y的坐标
                for j in range(len(x)):
                    shape['points'].append([x[j], y[j]])

                shape['shape_type'] =  data_ref['shapes'][0]['shape_type']
                shape['flags'] = data_ref['shapes'][0]['flags']
                shapes.append(shape)
    return shapes
        

def Coco2labelme(json_path,data_ref):
    with open(json_path,'r') as fp:
        data = json.load(fp)  # 加载json文件
        for img in data['images']:
            id = img['id']
            data_labelme={}
            data_labelme['version'] = data_ref['version']
            data_labelme['flags'] = data_ref['flags']

            data_labelme['shapes'] = labelme_shapes(data,data_ref,id)

            data_labelme['imagePath'] = img['file_name']

            data_labelme['imageData'] = None
            # ~ data_labelme['imageData'] = data_ref['imageData']

            data_labelme['imageHeight'] = img['height']
            data_labelme['imageWidth'] = img['width']
            file_name = data_labelme['imagePath']
            # 保存json文件
            json.dump(data_labelme,open('./%s.json' % file_name.split('.')[0],'w'),indent=4)
        return data_labelme

if __name__ == '__main__':
    root_dir = '/home/eason/PackSeg/'
    json_list = os.listdir(root_dir)
    #参考的json
    data_ref = reference_labelme_json()
    
    for json_path in json_list:
        if json_path.split('.')[-1] == 'json':
            print('当前文件： ', json_path)
            data_labelme= Coco2labelme(os.path.join(root_dir,json_path), data_ref)
            #file_name = data_labelme['imagePath']
            # 保存json文件
            #json.dump(data_labelme,open('./%s.json' % file_name.split('.')[0],'w'),indent=4) 
