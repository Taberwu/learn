<!--
 * @FilePath: jsslam_readme.md
 * @Author: Taberwu
 * @Version: 2.0
 * @Date: 2024-09-19 19:43:29
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-09-30 16:55:00
 * Copyright: 2024 Taberwu. All Rights Reserved.
 * @Descripttion: 
-->
 [TOC]   

# libjsslam  
libjsslam 定位算法代码仓库  
可以通过修改$./script/build$  中  `PLATFORM`：  
`<86_64_linux aarch64_linux,aarch64_qnx>`生成不同平台的动态库.     
## 目录结构:    
  
<table border="2" >
	<tr >
		<td>目录</td>
		<td colspan="2">主要包含文件</td>
        <td>备注</td>
	</tr>
    <tr >
        <td rowspan="13">applets</td>
        <td rowspan="4">config(配置文件)</td>
        <td>empty.json</td>
        <td>只启动qt可视化,用来标定相关功能 </td>
    </tr>
    <tr >
        <td>J5.json</td>
        <td>使用J5环视外部感知泊车建图定位功能</td>
    </tr>
    <tr >
        <td>joyson_id6.json</td>
        <td>ID6 #4号车 定位功能配置文件</td>
    </tr>
    <tr >
        <td>simulator.json</td>
        <td>仿真imu数据验证定位算法</td>
    </tr>
    <tr >
        <td rowspan="4">dataset(数据集)</td>
        <td>avm_id6_4_2024-07-29</td>
        <td>当前环视拼接标定数据(相机位置有变动)</td>
    </tr>
    <tr >
        <td>surroud_wheel_calib</td>
        <td>环视拼接中心与车辆后轴中心标定数据</td>
    </tr>
    <tr >
        <td>wheel_rtk_calib_id6_4.jsd</td>
        <td>RTK主从天线与车辆后轴中心标定数据</td>
    </tr>
    <tr >
        <td>imu_bsw_calib_id6_4.bag</td>
        <td>ID6 4号车imu内参标定数据</td>
    </tr>
    <tr >
        <td >map(泊车语义地图)</td>
        <td>shanghai.jsm</td>
        <td>上海地下两层地图</td>
    </tr>
    <tr >
        <td rowspan="3">models(模型)</td>
        <td>ddrnet</td>
        <td>语义分割模型</td>
    </tr>
    <tr >
        <td>mixvpr</td>
        <td>图像匹配定位模型(用于全局重定位)</td>
    </tr>
    <tr >
        <td>psd</td>
        <td>库位检测模型</td>
    </tr>
    <tr >
        <td colspan="2">main.cpp</td>
        <td>用于debug生成可执行二进制</td>
    </tr>
    <tr >
        <td >include</td>
        <td td colspan="2">agent.h</td>
        <td>对外接口头文件 </td>
    </tr>
    <tr >
        <td rowspan="60">include_internal/jsslam</td>
        <td rowspan="6">algorithm(通用算法)</td>
        <td> epipolar_geometry.h</td>
        <td> 计算单应矩阵和基础矩阵,选择重投影误差最小初始化单目<br>
        参考orbslam <a href="https://blog.csdn.net/shanpenghui/article/details/110133454" target="_blank" rel="noopener noreferrer">计算原理</a></td>
    </tr>
    <tr >
        <td> gradient_map.h</td>
        <td> 对输入数据离散建立梯度图,用于匹配</td>
    </tr>
    <tr >
        <td> grid_search_align.h</td>
        <td> 撒点,网格匹配</td>
    </tr>
    <tr >
        <td> imu_rectify.h</td>
        <td>  根据标定的 尺度系数,不正交系数和零偏 修正imu测量值</td>
    </tr>
    <tr >
        <td> map_save.h</td>
        <td> 泊车语义地图(关键帧)写文件/读文件</td>
    </tr>
     <tr >
        <td> surround_stitching.h</td>
        <td> 环视图像拼接映射</td>
    </tr>
    <tr >
        <td rowspan="8">calibration(标定)</td>
        <td> mono_calib.h</td>
        <td> 录取标定板标定数据,识别AprilTag 标定相机内参<br>
        参考AprilTag <a href="https://blog.csdn.net/wangmj_hdu/article/details/111233878" target="_blank" rel="noopener noreferrer">论文实现链接</a></td>
    </tr>
    <tr >
    <td> stereo_calib.h</td>
        <td> 读入双目相机图像数据,标定右相机相对左相机的外参<br>
        参考basalt <a href="https://zhuanlan.zhihu.com/p/447693693" target="_blank" rel="noopener noreferrer">标定原理</a></td>
    </tr>
    <tr >
     <td> surround_calib.h</td>
        <td> 读入四张环视鱼眼图像(PNG格式),手动配置标定特征点世界系和图像坐标文件(例:  <a href="./applets/dataset/avm_id6_4_2024-07-29/config.json" target="_blank" rel="noopener noreferrer">config.json</a>)
    </td>
    </tr> 
    <tr >
    <td> imu_bsm_calib.h</td>
        <td>录取对imu各轴进行静止和转动数据 标定imu的正交系数,尺度系数以及零偏<br>
        加速度计和陀螺采用 <a href="./documents/imu_bsm_model.pdf" target="_blank" rel="noopener noreferrer">参数模型</a></td>
    </tr>
    <tr >
    <td> imu_static_calib.h</td>
        <td>录取对imu静止和转动数据 标定imu的随机游走<br>
        参考 <a href="https://zhuanlan.zhihu.com/p/158927004" target="_blank" rel="noopener noreferrer">allan 方差辨识</a></td>
    </tr>
    <tr >
     <td> cam_imu_calib.h</td>
        <td> 读入 相机内参, 图像数据,imu数据,标定相机相对imu的外参<br>
        使用basalt库 <a href="https://zhuanlan.zhihu.com/p/447693693" target="_blank" rel="noopener noreferrer">基本标定原理</a></td>
    </tr>
    <td> wheel_calib.h</td>
        <td>录取差分定位数据与底盘 轮速,轮速脉冲,方向盘转角数据,手眼标定车辆运动模型到参数<br>
        车辆运动参数 <a href="./documents/wheel_calib.pdf" target="_blank" rel="noopener noreferrer">标定原理</a></td>
    </tr>
    <td> surrounding_wheel_calib.h</td>
        <td>环视拼接中心相对车辆运动模型原点外参标定<br>
       手动填写车辆运动中多个时刻库位角点图像坐标文件(例:<a href="./applets/dataset/surroud_wheel_calib/select_slot_24-04-29.json" target="_blank" rel="noopener noreferrer">select_slot_24-04-29.json</a>) 手眼标定参数</td>
    </tr>
    <tr >
        <td rowspan="5">camera(相机模型)</td>
        <td> i_camera.h</td>
        <td> camera的基类 声明 去畸变 归一化 投影 接口<br></td>
    </tr>
    <tr >
        <td> i_camera_system.h</td>
        <td> camera system的基类<br>  配置外参和描述子类型</td>
    </tr>
    <tr >
        <td> pinhole_camera.h</td>
        <td> 针孔相机模型<br></td>
    </tr>
    <tr >
        <td> equi_camera.h</td>
        <td> 鱼眼相机模型<br></td>
    </tr>
    <tr >
        <td> rectifie_camera.h</td>
        <td> 矫正相机模型<br></td>
    </tr>
     <tr >
        <td >config(参数配置)</td>
        <td> config.h</td>
        <td> 车辆传感器参数结构体(默认值为ID6 4号车相关传感器参数)<br></td>
    </tr>
    <tr >
        <td rowspan="7">feature(图像特征)</td>
         <td> i_feature_extractor.h</td>
        <td> 特征检测基类<br>
        声明 特征检测(solve) 和 金字塔(pyramid) 接口
         </td>
    </tr>
    <tr ><td> apriltag_extractor.h</td>
        <td> apriiltag 特征识别(用于相机内参标定)<br>
         参考AprilTag <a href="https://blog.csdn.net/wangmj_hdu/article/details/111233878" target="_blank" rel="noopener noreferrer">论文实现链接</a></td>
    </tr>
     <tr ><td> orb_extractor.h(对比测试)</td>
        <td> orb 特征提取与匹配<br>
         参考orbslam3 <a href="https://blog.csdn.net/xiaoma_bk/article/details/121223575" target="_blank" rel="noopener noreferrer">算法流程总结</a></td>
    </tr>
     <tr ><td> spp_extractor.h(用于slam全局描述子提取与匹配)</td>
        <td> Superpoint(spp) 特征提取<br>
         参考SuperPoint <a href="https://blog.csdn.net/private_Jack/article/details/132730345" target="_blank" rel="noopener noreferrer">算法流程总结</a></td>
    </tr>
    <tr ><td> feature_spacial_search.h(spp全局描述子查询)</td>
        <td> Superpoint 特征查询 <br></td>
    </tr>
    <tr ><td> spacial_matcher.h(spp全局描述子匹配)</td>
        <td> Superpoint 特征匹配<br>
      根据描述子匹配距离最小的特征点</td>
    </tr>
    <tr><td> parking_slot_detector.h(库位检测)</td>
        <td> 调用ncnn 库位检测 角点<br></td>
    </tr>
    <tr >
        <td rowspan="6">functional</td>
         <td> attitude_estimation.h</td>
        <td> 姿态估计<br>
            根据加速度计数据与重力对齐,采用误差卡尔曼递推估计姿态
         </td>
    </tr>
    <tr><td> global_localozation.h(全局重定位)</td>
        <td> <a href="#global_localozation">重定位算法流程</a><br></td>
    </tr>
    <tr><td> integration_sync.h</td>
     <td> 实现imu与wheel 数据时间同步</td>
    </tr>
    <tr><td> pipeline_ctrl.h</td>
     <td> 建图与定位模式切换控制</td>
    </tr> <td> surround_image_stitching.h</td>
     <td> 读取环视相机参数,生成lut, 拼接成avm图像</td>
    </tr>
    </tr> <td> surround_image_detection</td>
     <td> 对avm拼接图 分别调用库位检测和地面标识分割 输出检测分割图像</td>
    </tr>
     <tr >
        <td rowspan="5">fusion( <a href="#global_localozation">基于误差卡尔曼框架的组合定位</a>)[不再使用]</td>
        <td> eskf.h</td>
        <td> 误差卡尔曼框架<br>
            声明 输入量测(correct()), 更新(update**) 以及更新(predict) 的接口
         </td>
    </tr>
     <tr >
        <td> eskf_localization.h</td>
        <td> 基于误差卡尔曼的定位算法主程序<br>
            根据配置文件选用不同的量测,输出定位结果回调函数
         </td>
    </tr>
     <tr >
        <td> eskf_corrector.h</td>
        <td> 计算轮速,gnss和零速 误差的修正的 增益和协方差<br>
         </td>
    </tr>
    <tr >
        <td> eskf_predictor.h</td>
        <td> 基于IMU数据的预测更新<br>
         </td>
    </tr>
    <tr >
        <td> zupt_detection.h</td>
        <td> 零速检测<br>
         </td>
    </tr>
    <tr >
        <td rowspan="4">graphics(基于Qt实现的可视化)</td>
        <td> graphics.h</td>
        <td> 可视化主程序<br>
            实现依赖声明和页面的注册
         </td>
    </tr>
     <tr >
        <td> graphics_context.h</td>
        <td> 增加Tab的实现<br></td>
    </tr>
     <tr >
        <td> mainwidget.h</td>
        <td> 主控件的布局设置 <br></td>
    </tr>
     <tr >
        <td> scene_cam_imu_calib.h</td>
        <td> 可视化cam_imu_calib<br></td>
    </tr>
    <tr >
        <td rowspan="3">localization(基于因子图定位算法)[当前使用版本]</td>
        <td> fusion_localization.h</td>
        <td> 可视化主程序<br>
         </td>
    </tr>
    
</table>


### 全局重定位
<span id="global_localozation">global_localozation</span>
 ```mermaid
flowchart LR
  subgraph RoughmapMatch 
    direction LR
    proc1[/descripition match /] --Y--> proc2[/continuity check/]
  end
  subgraph objectsMatch  
    proc3[/slot match/]
  end
datasource1(localmap) 
datasource2(offlinemap)
output([globalLocalization])
datasource2 --mapkeyframes--> proc1
datasource1 -- keyframes --> proc1 
proc2 -- nearbyslots --> proc3
datasource1 -- slots --> proc3-->output
 ```


 ### 基于ESKF的组合定位
<span id="ESKF_localozation">fusion</span>


```mermaid
sequenceDiagram 
participant G as gnss
participant W as wheel
participant S as stationary_check
participant A as InertialPredictor
G->> A: gnsss_corrector
W-->> A: wheel_corrector
S-->> A: stationary_corrector
```

```mermaid
flowchart LR
datasource3(SolvedGnss)
datasource1(imu) 
datasource2(wheel)

output([fusionLocalization])
  subgraph Initialize
    direction TB
    proc1[/Cache imu_data/]
    proc2[/Calc yaw by gnss/]
    proc3[/Calc Rotaion&&Bais/]
    init_result([origin_localization])
  end
  subgraph InertialPredictor
     proc4[/predict/]
  end

  subgraph corrector
    proc5[/gnsss_corrector/]
    proc6[/wheel_corrector/]
    proc7[/stationary_corrector/]
  end

datasource1 --> proc1 --> proc3 
datasource3 --> proc2 --> proc3 
proc3 --> init_result --> proc4 --> output

datasource1 --> proc7 --> proc4
datasource3 --> proc5 --> proc4
datasource2 --> proc6 --> proc4

 ```
















