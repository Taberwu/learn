<!--
 * @FilePath: imu_calibration.md
 * @Author: Taber.wu
 * @Date: 2022-10-31 08:41:11
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-11-04 16:29:48
 * Copyright: 2022 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
-->
# A Novel IMU Extrinsic Calibration Method for Mass Production Land Vehicles     
## 关键词：  
车载传感器 联合状态参数估计   

## 误差模型：    
比力/角速率误差：
$$\hat{f}^m_{im}=f^m_{im}+b_f \\
\hat{\omega}^m=\omega^m+b_{\omega}$$
## 失准角估计（rool pitch yaw)  
### roll pitch失准角估计：       
通过同一位置两次相反的车辆朝向的imu的数值，估计imu安装的误差角：  
$$\begin{aligned}
\phi_{vg} &= \frac{\phi_{ng,1}+\phi_{ng,2}-\phi_{rv,1}-\phi_{rv,2}}{2}  \\
\theta_{vg} &= \frac{\theta_{ng,1}+\theta_{ng,2}-\theta_{rv,1}-\theta_{rv,2}}{2}   
\end{aligned}$$  
假设当前位置坡度$\phi_{ng}$和侧倾角$\theta_{ng}$时，保证位置相同时，两次测量的真值$\phi_{ng}$和$\theta_{ng}$可以认为互为相反数的固定值，可以直接记其和为0,从而不依赖高精度测量设备得到roll pitch的失准角  
### yaw失准角估计：  
yaw失准角的估计需要运动状态下，根据运动学模型：  
$$v^r_{er,y}\approx I_r\omega ^r_Z-SGv^r_{er,x}f^r_{ir,y^`}$$
由真值系统的roll值代表道路平面，得到：  
$$v^v_{ev,y}=v^r_{er,y$$  
当车辆理想地在水平路面直线行驶时