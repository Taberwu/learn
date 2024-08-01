/****************************************************************************************
 * @FilePath: manifcalcu.hpp
 * @Author: Taber.wu
 * @Date: 2024-07-05 09:43:51
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-07-05 10:13:58
 * @Copyright: 2024 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef __MANIFLU_TEST_HPP__
#define __MANIFLU_TEST_HPP__


#include <manif/SE3.h>
#include <Eigen/Eigen>
#include <Eigen/Geometry> 
#include <iostream>

namespace manifcalcu {
    class ManifTest{
        public:
        ManifTest(){}
        ~ManifTest() = default;

        void actTest(){
            Eigen::Vector3d p1 (3,3,0);
            
            Eigen::Vector3d pt(1, -1, 0);

            manif::SE3d tba(Eigen::Vector3d(1,0,0),Eigen::Quaterniond(1.,0.,0.,0.));

            manif::SE3d mv(Eigen::Vector3d(2,2,0),Eigen::Quaterniond(sqrt(2)/2.,0.,0.,sqrt(2)/2));

            

            Eigen::Vector3d target1 = mv.inverse().act((p1));

            Eigen::Vector3d target2 = tba.act(pt);
            Eigen::Vector3d target3 = tba.act(target1);

            std::cout<<"t1 "<<target1.transpose()<<std::endl;
            std::cout<<"t2 "<<target2.transpose()<<std::endl;
            std::cout<<"t3 "<<target3.transpose()<<std::endl;
        }


    };
}


#endif