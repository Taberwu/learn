/****************************************************************************************
 * @FilePath: readfiledata.cpp
 * @Author: Taber.wu
 * @Date: 2023-10-27 14:49:06
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-10-27 15:04:03
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#include "readfiledata.h"

namespace jsos{
    FileDataReader::FileDataReader(const std::string filename){
        datafile_.open(filename.c_str(), std::ios::in);
        if(!datafile_.is_open()){
            printf("fail to open imu data file\n");
        }
    }

    void FileDataReader::readimudata(std::deque<std::shared_ptr<ImuData>> &imudatas){
        
        std::string buff = "";
        while(std::getline(datafile_, buff)){
            ImuData imu_data;
            std::stringstream ss(buff);
            ss >> imu_data.timestamp >> imu_data.acce.x() >> imu_data.acce.y() >> imu_data.acce.z()
               >> imu_data.gyro.x() >> imu_data.gyro.y() >> imu_data.gyro.z();
            auto data = std::make_shared<ImuData>(imu_data);
            imudatas.push_back(data);
            buff.clear();
            ss.str("");
        }
    }
}