#### 本方案用于解决建筑平面图灯具点位手工输入编码的速度慢，准确度不稳定且耗费大量时间和人力的痛点，针对此需求，在此提出使用百度飞桨paddle开源模型 PaddleOCR + ONNXRuntime 加速推理的解决方案

#### 一、运行环境
Visual studio 2019 + QT5.15.2（msvc）

上传内存有限，第三方库和模型就不上传到仓库中，首先确保3rdparty和model放在OCRFastPoint同级目录中，并且在visualstudio中配置好对应的lib路径和dll路径

#### 通过网盘分享的文件：model+3rdparty
链接: https://pan.baidu.com/s/1GgFHTQv6OcLdap_zM2v59w?pwd=dd5t 提取码: dd5t 
--来自百度网盘超级会员v6的分享

#### 二、模型准备 (PaddleOCR)



PaddleOCR 原始模型是 .pdparams 格式，需要先转为推理模型，再转为 ONNX 格式。

导出推理模型：使用 PaddleOCR 提供的 tools/export_model.py 得到 inference.pdmodel 和 inference.pdiparams。

转换为 ONNX：使用 paddle2onnx 工具。

#### 三、图像预处理(OpenCV)

读取与缩放：使用 cv::imread 加载图像，并按照模型要求进行 Resize（如检测模型通常要求是 32 的倍数）。

色彩空间转换：PaddleOCR 通常要求 BGR 转 RGB。

归一化：将像素值从 [0, 255]映射到 [0, 1]，并进行均值（Mean）和标准差（Std）处理

通道变换：从 HWC（OpenCV 默认）转换为 CHW（ONNX Runtime 要求）

#### 四、模型推理 (ONNX Runtime)

通过 ONNX Runtime 调用硬件加速（如 CPU 上的 OpenVINO 或 GPU 上的 CUDA）。本案例考虑大部分调试人员使用的是不带独立显卡的笔记本，故使用的是cpu做硬件加速

#### 五、后处理 (OpenCV)

把处理好的数字信息转换成字符串返回交互界面

<img width="1920" height="1200" alt="png" src="https://github.com/user-attachments/assets/f0b859b1-2563-4e56-93e4-4f0fed8c8a5f" />

<img width="1920" height="1200" alt="png" src="https://github.com/user-attachments/assets/39ae0613-2276-4477-b13e-a0636cbb7137" />
<img width="250" height="77" alt="test" src="https://github.com/user-attachments/assets/c9cfdc83-7a60-4584-b9fc-f197d3d1153f" />

