<!--
 * @FilePath: Pf_HDLoc.md
 * @Author: Taber.wu
 * @Date: 2022-10-18 09:37:39
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-10-18 17:24:37
 * Copyright: 2022 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
-->
# HDMI-Loc: Exploiting High Definition Map Image for Precise Localization via Bitwise Particle Filter

## 关键词：
***6-DOF localization、8-bit representation for road information、高精度语义地图***   


## 方法:
论文中采用矢量地图(主要包括车道线、停止线、路边方向标识牌和路面标识)、深度相机感知数据和轮速计(编码盘差速轮模型)，主要分为3个步骤。  
### 步骤1： 感知处理与 高精度地图数据8bit预处理转换  



