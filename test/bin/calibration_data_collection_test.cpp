/**
****************************************************************************************
 * @FilePath: calibration_data_collection_test.cpp
 * @Author: Taber.wu
 * @Date: 2022-10-08 15:26:57
 * @LastEditors: 
 * @LastEditTime: 2022-10-08 15:26:57
 * @Copyright: 2022 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************
*/
#include <chrono>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
#include <random>

#include <jsslam/calibration/data_collection.h>

template<typename T>
class MockDataCollection :public jsos::algorithm::DataCollection{
public:
~MockDataCollection()  noexcept;

MOCK_METHOD4_T(receiveMsgHandler,void(const std::shared_ptr<T>,const jsos::algorithm::DataSource,const double,const std::uint32_t));
};
TEST(DataCollection, receiveMsgHandler)
{
   auto options = std::make_shared<jsos::algorithm::CalibrationCommon::Options>();
   options->calibration_collection_time = 30*1000*1000*1000;
   options->imu_period = 10*1000*1000;
   options->rtk_period = 1*1000*1000*1000;
   options->wheel_period = 20*1000*1000;
   options->pluse_per_cricle = 86;
   
   auto wheelMsg = std::make_shared<jsos::message::WheelMessage>();
   auto imuMsg = std::make_shared<jsos::message::LocalizationMessage>();
   auto rtkMsg =  std::make_shared<jsos::message::InspvaMessage>();
   for(int i = 0;i < 40000;i++){
    imuMsg->setLLA(Eigen::Vector3d(31.1543+1.0*i/10000,122.4512-1.0*i/10000,3));
    imuMsg->setOrigin(Eigen::Vector3d(31.1543,122.4512,3));
    imuMsg->setOmega(Eigen::Vector3d((random()%10)/100.0,(random()%10)/100.0,(random()%50)/100.0));
    imuMsg->setTimestamp(i*10*1000*1000);
    imuMsg->setRotation(Eigen::Quaterniond::UnitRandom());
    
   }

    ASSERT_TRUE(true);
}
