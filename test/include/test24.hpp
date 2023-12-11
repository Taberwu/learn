/****************************************************************************************
 * @FilePath: test24.hpp
 * @Author: Taber.wu
 * @Date: 2023-09-01 13:20:21
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-09-01 13:57:37
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef __MANIF_TEST_HPP__
#define __MANIF_TEST_HPP__

#include <Eigen/Eigen>
#include <Eigen/Core>
#include <Eigen/src/Geometry/Quaternion.h>
#include <manif/manif.h>
#include <iostream>

namespace maniftest{

class ManifTest{
public:
    ManifTest(const Eigen::Vector3d euler):euler_(euler)
    {

    }
    void test(){
        auto q = manif::exp(manif::SO3Tangent<double>(euler_)).rotation();
        std::cout<<q<<std::endl;
    }
    Eigen::Vector3d euler_;

};
}


#endif