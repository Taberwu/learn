/****************************************************************************************
 * @FilePath: message.h
 * @Author: Taber.wu
 * @Date: 2023-10-20 15:35:31
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-10-20 15:38:13
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#pragma once
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace jsos{

struct ImuData {
    std::uint64_t timestamp;
    Eigen::Vector3d acce;
    Eigen::Vector3d gyro;
};

}