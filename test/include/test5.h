/**
****************************************************************************************
 * @FilePath: test5.h
 * @Author: Taber.wu
 * @Date: 2023-01-13 10:27:14
 * @LastEditors: 
 * @LastEditTime: 2023-01-13 10:27:14
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************
*/
#ifndef TEST_5_H_
#define TEST_5_H_
#include <iostream>
#include <eigen3/Eigen/Core>
#include <GeographicLib-1.51/include/GeographicLib/Geocentric.hpp>
#include <GeographicLib-1.51/include/GeographicLib/Constants.hpp>
#include <GeographicLib-1.51/include/GeographicLib/GravityModel.hpp>
#include <GeographicLib-1.51/include/GeographicLib/GravityCircle.hpp>
#include <GeographicLib-1.51/include/GeographicLib/NormalGravity.hpp>

#define DEGREE2RAD M_PI/180.0

class TestFv{
public:
    TestFv(double lat=31.4356758, double height = 100.0):lat_(lat), height_(height)
    {
        Re = 6378135.072;
        e  = 1.0/298.257223563;
        g0 = 9.7803267714;
        gk1 = 0.00193185138639;
        gk2 = 0.00669437999013;
        double igf = 9.780327 * (1+0.0053024 * pow(sin(lat * 180.0/M_PI),2)-0.0000058 * pow(sin(2 * lat * 180.0/M_PI),2));
        double fac = -3.086 * 1e-6;
        double g_test1 = igf+fac;
        double g_test2 = g0*(1.0+gk1*pow(sin(lat*DEGREE2RAD),2))*(1.0-2.0*height/Re)/sqrt(1.0-gk2*pow(sin(lat*DEGREE2RAD),2));
        
        GeographicLib::NormalGravity g_test3(GeographicLib::Constants::WGS84_a(), GeographicLib::Constants::WGS84_GM(),
                                            GeographicLib::Constants::WGS84_omega(),GeographicLib::Constants::WGS84_f());
        double gz,gn;
        g_test3.Gravity(lat_,height_,gn,gz);
        auto g_test3_norm = sqrt(gn*gn+gz*gz);
        auto g_test3_z = gz;
        std::cout<<"compare test g"<<g_test1<<"  "<<g_test2<<" "<<g_test3_norm<<" "<<g_test3_z<<std::endl;
    }



private:
    double Re;
    double e;
    double g0;
    double gk1;
    double gk2;
    double lat_;
    double height_;
};

class TestSix{
public:
    TestSix(){}
    template<typename T>
    bool test(Eigen::Vector3d a, Eigen::Matrix3f b){
        auto test_a = Eigen::Matrix<T, 3, 1>(a[0], a[1], a[2]); 
        Eigen::Matrix<T, 3, 3> test_b ;
        test_b << b(0,0),b(0,1),b(0,2),b(1,0),b(1,1),b(1,2),b(2,0),b(2,1),b(2,2);
        std::cout<<test_a<<std::endl;
        std::cout<<test_b<<std::endl;
        return true;
    }

};



#endif