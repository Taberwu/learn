<!--
 * @FilePath: jsslam_readme.md
 * @Author: Taberwu
 * @Version: 2.0
 * @Date: 2024-09-19 19:43:29
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-09-20 18:17:55
 * Copyright: 2024 Taberwu. All Rights Reserved.
 * @Descripttion: 
-->
 [TOC]   

# libjsslam  
libjsslam 定位算法代码仓库  
可以通过修改$./script/build$  中  `PLATFORM`：  
`<86_64_linux aarch64_linux,aarch64_qnx>`生成不同平台的动态库.     
* 目录结构:    
  
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
        <td rowspan="13">include_internal/jsslam</td>
        <td rowspan="13">algorithm(通用算法)</td>
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
        <td> map_save.h</td>
        <td> 泊车语义地图(关键帧)写文件/读文件</td>
    </tr>
</table>

