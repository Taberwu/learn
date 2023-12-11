/****************************************************************************************
 * @FilePath: main.cpp
 * @Author: Taber.wu
 * @Date: 2023-10-19 11:02:28
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-10-27 17:20:46
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#include <memory>


#include "readfiledata.h"
#include "alignfine.h"



int main(int argc, char **argv){
    auto reader = std::make_shared<jsos::FileDataReader>("/home/joyson/Desktop/workspace/learn/imu_align/data/imudata.txt");
    Eigen::Vector3d lla = Eigen::Vector3d(31.070222652,121.363221640,4.44772);
    auto align = std::make_shared<jsos::Alignment>(lla);
    std::deque<std::shared_ptr<jsos::ImuData>> imudatas;
    imudatas.clear();
    reader->readimudata(imudatas);
    printf("imudata size %lu \n", imudatas.size());
    // printf("back data %lu  [%.7f %.7f %.7f] [%.7f %.7f %.7f]\n", imudatas.back()->timestamp,
    // imudatas.back()->acce.x(), imudatas.back()->acce.y(), imudatas.back()->acce.z(),
    // imudatas.back()->gyro.x(), imudatas.back()->gyro.y(), imudatas.back()->gyro.z());
    Eigen::Vector3d euler = Eigen::Vector3d::Zero();
    Eigen::Quaterniond rot = Eigen::Quaterniond::Identity();
    align->coarseAlign(imudatas, euler, rot);
    std::cout<<euler.transpose()<<std::endl;
}