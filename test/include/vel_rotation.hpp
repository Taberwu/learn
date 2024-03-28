/****************************************************************************************
 * @FilePath: vel_rotation.hpp
 * @Author: Taber.wu
 * @Date: 2024-02-06 09:50:40
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-02-06 10:23:43
 * @Copyright: 2024 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef _VEL_RATATION_TEST_HPP_
#define _VEL_RATATION_TEST_HPP_

#include <manif/SO3.h>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Eigen>
#include "rotation.hpp"
#include <iostream>

class VelRotation{
public:
    VelRotation(){}
    ~VelRotation(){}
    void test(){
        Eigen::Vector3d init_euler(0., 0., 45.);
        Eigen::Vector3d vel (2., 0., 0.);
        init_euler = M_PI/180.*init_euler;
        Eigen::Quaterniond init_rot = jsos::utility::eulerAngle2Quaternion(init_euler);

        Eigen::Vector3d velocity = init_rot * vel;
        Eigen::Vector3d qv(0., 0., 0.);
        qv = M_PI/180. * qv;
        auto test_rot1 = init_rot * manif::exp(manif::SO3Tangentd(qv)).rotation();
        Eigen::Vector3d test_vel1 = test_rot1 * vel;

        Eigen::Vector3d test_vel2  = init_rot.matrix() *  manif::exp(manif::SO3Tangentd(qv)).rotation() * init_rot.matrix().transpose() * velocity;

        Eigen::Vector3d test_vel3  = init_rot *  manif::exp(manif::SO3Tangentd(qv)).rotation() * init_rot.conjugate() * velocity;
        
        std::cout<<"vel 1 "<<test_vel1.transpose()<<std::endl;
        std::cout<<"vel 2 "<<test_vel2.transpose()<<std::endl;
        std::cout<<"vel 3 "<<test_vel3.transpose()<<std::endl;

    }
};

#endif