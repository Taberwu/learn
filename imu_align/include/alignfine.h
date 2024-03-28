/****************************************************************************************
 * @FilePath: alignfine.h
 * @Author: Taber.wu
 * @Date: 2023-10-19 11:05:08
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-01-26 11:31:24
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#pragma once

#include <deque>
#include <memory>
#include <iostream>

#include "rotation.hpp"
#include "message.h"

#include <manif/SO2.h>
#include <GeographicLib/LocalCartesian.hpp>
#include <GeographicLib/GravityModel.hpp>
#include <GeographicLib/Constants.hpp>

namespace jsos{
class Alignment{
public:
    Alignment(){}
    Alignment(const Eigen::Vector3d &origin_lla);
    virtual ~Alignment() = default;
    void coarseAlign(const std::deque<std::shared_ptr<ImuData>> &data, Eigen::Vector3d &euler,
                    Eigen::Quaterniond &rot);

    void kalman_optimate(const std::deque<std::shared_ptr<ImuData>> &data, Eigen::Vector3d &euler,
                    Eigen::Quaterniond &rot);

private:
    std::deque<std::shared_ptr<ImuData>> alignment_datas_;
    Eigen::Vector3d origin_lla_;
    Eigen::Vector3d gravity_;
};
}