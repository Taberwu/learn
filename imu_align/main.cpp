/****************************************************************************************
 * @FilePath: main.cpp
 * @Author: Taber.wu
 * @Date: 2023-10-19 11:02:28
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-12-18 16:25:53
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#include <memory>


#include "readfiledata.h"
#include "alignfine.h"



int main(int argc, char **argv){
    auto reader = std::make_shared<jsos::FileDataReader>("/home/joyson/docker_data/imu.csv");
    Eigen::Vector3d lla = Eigen::Vector3d(31.0704164905105,121.363123056435,4.91496951485042);
    auto align = std::make_shared<jsos::Alignment>(lla);
    std::deque<std::shared_ptr<jsos::ImuData>> imudatas;
    imudatas.clear();
    reader->readimudata(imudatas);
    printf("imudata size %lu \n", imudatas.size());
    Eigen::Vector3d euler = Eigen::Vector3d::Zero();
    Eigen::Quaterniond rot = Eigen::Quaterniond::Identity();
    align->coarseAlign(imudatas, euler, rot);
    std::cout<<euler.transpose()<<std::endl;
}