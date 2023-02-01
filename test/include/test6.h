/**
****************************************************************************************
 * @FilePath: test6.h
 * @Author: Taber.wu
 * @Date: 2023-01-28 08:58:04
 * @LastEditors: 
 * @LastEditTime: 2023-01-28 08:58:04
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************
*/
#ifndef LAERN_TEST_6_H_
#define LAERN_TEST_6_H_

#include <iostream>

#include <GeographicLib-1.51/include/GeographicLib/Geocentric.hpp>
#include <GeographicLib-1.51/include/GeographicLib/Constants.hpp>
#include <GeographicLib-1.51/include/GeographicLib/NormalGravity.hpp>
#include <eigen3/Eigen/Core>



class TESTSX{
public :
    TESTSX(){}
    TESTSX(double lat, double height, Eigen::Vector3d vel):lat_(lat), height_(height), vel_(vel)
    {
        GeographicLib::NormalGravity g(GeographicLib::Constants::WGS84_a(), GeographicLib::Constants::WGS84_GM(),
                                       GeographicLib::Constants::WGS84_omega(),GeographicLib::Constants::WGS84_f());
        double gn;
        g.Gravity(lat_,height_,gn, gavrity_);
        omega_ = 2.0 * M_PI/24/3600.0;
        lat_ *= DEGREE2RAD;
    }
    ~TESTSX(){}
    void test(){
        
        Eigen::Vector3d omega_e_n (0, omega_ * cos(lat_), omega_ * sin(lat_));
        double e_02 = 2.0 * GeographicLib::Constants::WGS84_f() - std::pow( GeographicLib::Constants::WGS84_f(), 2);
        
        double rnh = GeographicLib::Constants::WGS84_a()/sqrt(1 - e_02 * sin(lat_)) + height_;
        double rmh = (rnh - height_) * (1-e_02)/(1 - e_02 * std::pow(sin(lat_), 2)) + height_;
        Eigen::Vector3d omega_en_n = Eigen::Vector3d(-vel_[1]/rmh , vel_[0]/rnh , vel_[0] * tan(lat_)/rnh);

        auto diff = (2 * omega_e_n + omega_en_n).cross(vel_);
        std::cout<<"diff :"<<std::endl;
        std::cout<<diff.transpose()<<std::endl;
        auto diff_z = 0.5 * diff[2] * 80.0 * 80.0;
        std::cout<<" z error :"<<diff_z<<std::endl; 
    }


private:
    double lat_;
    double height_;
    double omega_;
    double gavrity_;
    Eigen::Vector3d vel_;
};


#endif