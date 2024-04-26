/****************************************************************************************
 * @FilePath: cacalute.hpp
 * @Author: Taber.wu
 * @Date: 2024-04-26 10:11:32
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-04-26 11:26:33
 * @Copyright: 2024 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef MATH_RESULT_HPP_
#define MATH_RESULT_HPP_

#include <Eigen/Eigen>
#include <Eigen/Core>
#include <Eigen/src/Geometry/Quaternion.h>
#include <cmath>
#include <iostream>
#include <string>

namespace cal{
 struct DataWY{
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    public:
        Eigen::Vector3d s_w_ex_t[3];
        Eigen::Quaterniond s_w_ex_q[3];
        DataWY(Eigen::Vector3d s_t, Eigen::Vector3d w_t,Eigen::Vector3d ex_t,double s_yaw,double w_yaw, Eigen::Quaterniond ex_q){
            s_w_ex_t[0] = s_t; s_w_ex_t[1] = w_t; s_w_ex_t[2] = ex_t;
            double s = s_yaw /180.*M_PI;
            double w = w_yaw/180.*M_PI;
            s_w_ex_q[0] = Eigen::Quaterniond(cos(0.5 * s), 0., 0., sin(0.5 * s)); 
            s_w_ex_q[1] = Eigen::Quaterniond(cos(0.5 * w), 0., 0., sin(0.5 * w)); 
            s_w_ex_q[2] = ex_q;

        }
    };

class Result{
public:
    Result(){}
    ~Result(){}

    static void test(Eigen::Vector3d *s_w_ex_t, Eigen::Quaterniond *s_w_ex_q ){
        auto s_t = s_w_ex_t[0];     auto s_q = s_w_ex_q[0];
        auto w_t = s_w_ex_t[1];     auto w_q = s_w_ex_q[1];
        auto ex_t = s_w_ex_t[2];    auto ex_q = s_w_ex_q[2];

        printf("trans s[%.7f %.7f] w[%.7f %.7f]  %.7f\n", s_t.x(), s_t.y(), w_t.x(),w_t.y(), std::acos(s_q.w())*2/M_PI*180.);
    
        auto cal_e_t = ((ex_q.conjugate()) * s_t);
        auto cal_t1 = cal_e_t + ex_t;
        auto cal_t2 =(w_q.conjugate() * ex_t);
        auto cal_t =  cal_t1 - cal_t2;
        auto delta = w_t - cal_t;
        

        // std::cout<<" cal_e_t  "<< cal_e_t.transpose()<<std::endl;
        std::cout<<" cal_t1  "<< cal_t1.transpose()<<std::endl;
        std::cout<<" cal_t2  "<< cal_t2.transpose()<<std::endl;
        std::cout<<" cal_t  "<< cal_t.transpose()<<std::endl;
        std::cout<<" delta  "<<delta.transpose()<<std::endl;
    }
};

}
#endif