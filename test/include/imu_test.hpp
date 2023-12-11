/****************************************************************************************
 * @FilePath: imu_test.hpp
 * @Author: Taber.wu
 * @Date: 2023-09-11 09:23:24
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-09-11 14:04:25
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef _IMU_TSET_HPP_
#define _IMU_TSET_HPP_
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <Eigen/Eigen>
#include <Eigen/Core>
#include <Eigen/src/Geometry/Quaternion.h>
#include <manif/manif.h>


namespace imu_test{

template <typename scalar>
static Eigen::Matrix<scalar, 3, 1> quaternion2EeulerAngle(const Eigen::Quaternion<scalar>& quaternion)
{
    Eigen::Matrix<scalar, 3, 1> rotation = Eigen::Matrix<scalar, 3, 1>::Zero();
    // roll (x-axis rotation)
    scalar sinr_cosp = 2 * (quaternion.w() * quaternion.x() + quaternion.y() * quaternion.z());
    scalar cosr_cosp = 1 - 2 * (quaternion.x() * quaternion.x() + quaternion.y() * quaternion.y());
    rotation.x() = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    scalar sinp = 2 * (quaternion.w() * quaternion.y() - quaternion.z() * quaternion.x());
    if (std::abs(sinp) >= 1)
        rotation.y() = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
    else
        rotation.y() = std::asin(sinp);

    // yaw (z-axis rotation)
    scalar siny_cosp = 2 * (quaternion.w() * quaternion.z() + quaternion.x() * quaternion.y());
    scalar cosy_cosp = 1 - 2 * (quaternion.y() * quaternion.y() + quaternion.z() * quaternion.z());
    rotation.z() = std::atan2(siny_cosp, cosy_cosp);

    return rotation;
}

template <typename scalar>
static Eigen::Quaternion<scalar> angleAxis2Quaternion(const Eigen::Matrix<scalar, 3, 1>& angleaxis)
{
    scalar theta = angleaxis.norm();
    if(theta < std::numeric_limits<scalar>::epsilon()){
        return Eigen::Quaternion<scalar>::Identity();
    }
    Eigen::Matrix<scalar, 3, 1> axis = angleaxis / theta;
    scalar w = std::cos(0.5 * theta);
    scalar tsin = std::sin(0.5 * theta);
    scalar x = axis[0] * tsin;
    scalar y = axis[1] * tsin;
    scalar z = axis[2] * tsin;
    return Eigen::Quaternion<scalar>(w, x, y, z).normalized();
}

template <typename scalar>
static Eigen::Quaternion<scalar> eulerAngle2Quaternion(const Eigen::Matrix<scalar, 3, 1>& euler)
{
    // https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
    scalar yaw = euler.z();
    scalar pitch = euler.y();
    scalar roll = euler.x();

    scalar cy = std::cos(yaw * 0.5);
    scalar sy = std::sin(yaw * 0.5);
    scalar cp = std::cos(pitch * 0.5);
    scalar sp = std::sin(pitch * 0.5);
    scalar cr = std::cos(roll * 0.5);
    scalar sr = std::sin(roll * 0.5);

    scalar w = cy * cp * cr + sy * sp * sr;
    scalar x = cy * cp * sr - sy * sp * cr;
    scalar y = sy * cp * sr + cy * sp * cr;
    scalar z = sy * cp * cr - cy * sp * sr;
    return Eigen::Quaternion<scalar>(w, x, y, z);
}

class Predictor{
public:
    Predictor(std::string &filepath){
        is_fisrtline_ = true;
        lines_ = 0;
        datafile_.open(filepath, std::ios::in);
        resultfile_.open("/home/joyson/docker_data/eskf_debug/onlyImuRes.csv", std::ios::out|std::ios::trunc);
        if(!datafile_.is_open()){
            printf("fail to open data source file\n");
        }
    }
    ~Predictor(){
        datafile_.close();
        resultfile_.close();
    }
    void test(){
        std::string buff("");
        Eigen::Vector3d ba = Eigen::Vector3d::Zero();
        Eigen::Vector3d bg = Eigen::Vector3d::Zero();
        Eigen::Vector3d op_trans = Eigen::Vector3d::Zero();
        Eigen::Vector3d op_vel = Eigen::Vector3d::Zero();
        Eigen::Quaterniond op_rotation = Eigen::Quaterniond::Identity();
        while(getline(datafile_, buff)){
            // if(lines_ > 60000) break;
            if(is_fisrtline_){
                getOrigin(buff);
                is_fisrtline_ = false;
                lines_++;
            }
            else{
                std::uint64_t current_stmap;
                Eigen::Vector3d curr_acc, cur_gyr;
                
                parse(buff, curr_acc, cur_gyr, current_stmap);
                double dt = (current_stmap - last_stamp_) * 1e-9;
                Eigen::Vector3d imu_acc = 0.5 * (curr_acc + last_acce_);
                Eigen::Vector3d imu_gyr = 0.5 * (cur_gyr + last_omega_);
                Eigen::Vector3d acce = last_rotation_ * (imu_acc - ba) + Eigen::Vector3d(0.f ,0.f, -9.81);
                printf("%lu %.7f %.7f %.7f =  %.7f  %.7f  %.7f  %.7f* %.5f %.5f %.5f -%.5f %.5f %.5f\n", current_stmap, acce.x(), acce.y(), acce.z(),
    last_rotation_.w(),last_rotation_.x(), last_rotation_.y(), last_rotation_.z(), imu_acc.x(), imu_acc.y(), imu_acc.z(), ba.x(), ba.y(), ba.z());

                op_trans = last_trans_ + last_velocity_ * dt + 0.5 * acce * dt * dt;
                op_vel =last_velocity_ + acce * dt;
    //             printf("%lu %.5f %.5f %.5f = %.5f %.5f %.5f + %.5f %.5f %.5f *%.5f\n", current_stmap, op_vel.x(), op_vel.y(), op_vel.z(),
    // last_velocity_.x(), last_velocity_.y(), last_velocity_.z(), acce.x(), acce.y(), acce.z(), dt);
                Eigen::Vector3d qv = (imu_gyr - bg) * dt;
                op_rotation = last_rotation_ * angleAxis2Quaternion<double>(qv);
                //output.rotation = input.rotation * manif::exp(manif::SO3Tangentd(qv)).rotation();
                op_rotation.normalize();

                auto loc_euler = quaternion2EeulerAngle(op_rotation);
                loc_euler = loc_euler*180.f/M_PI;
                resultfile_<<current_stmap<<" "<<op_trans.x()<<" "<<op_trans.y()<<" "<<op_trans.z()<<" "<<op_vel.x()<<" "<<op_vel.y()<<" "<<op_vel.z()<<" "
                            <<loc_euler.x()<<" "<<loc_euler.y()<<" "<<loc_euler.z()<<std::endl;

                last_trans_ = op_trans;
                last_velocity_ = op_vel;
                last_rotation_ = op_rotation;
                last_acce_ = curr_acc;
                last_omega_ = cur_gyr;
                last_stamp_ = current_stmap;
                lines_++;
            }
            if(lines_%500 == 0){
                // printf("read file line %lu\n",lines_);
            }
             usleep(100);
        }
    }

protected:
    void parse(const std::string input, Eigen::Vector3d &acce, Eigen::Vector3d &omega, std::uint64_t &timestamp){
        Eigen::Vector3d euler, trans, vel;
        std::stringstream ss(input);
        ss>>timestamp>>trans.x()>>trans.y()>>trans.z()>>vel.x()>>vel.y()>>vel.z()>>acce.x()>>acce.y()>>acce.z()>>omega.x()>>omega.y()>>omega.z()>>euler.x()>>euler.y()>>euler.z();
        // std::cout<<timestamp<<"  ["<<trans.x()<<","<<trans.y()<<"]  ["<<acce.x()<<","<<acce.y()<<", "<<omega.z()<<"]"<<std::endl;
    }

    void getOrigin(const std::string input){
        Eigen::Vector3d euler, trans, vel;
        std::stringstream ss(input);
        ss>>last_stamp_>>trans.x()>>trans.y()>>trans.z()>>last_velocity_.x()>>last_velocity_.y()>>last_velocity_.z()>>last_acce_.x()>>last_acce_.y()>>last_acce_.z()>>last_omega_.x()>>last_omega_.y()>>last_omega_.z()>>euler.x()>>euler.y()>>euler.z();
        last_trans_ = trans;
        euler = euler *M_PI/180.f;
        last_rotation_ = eulerAngle2Quaternion(euler);
    }

private:
    std::fstream datafile_;
    std::fstream resultfile_;
    bool is_fisrtline_;
    Eigen::Vector3d last_trans_;
    Eigen::Vector3d last_acce_;
    Eigen::Vector3d last_omega_;
    Eigen::Quaterniond last_rotation_;
    Eigen::Vector3d last_velocity_;
    std::uint64_t last_stamp_;
    std::uint64_t lines_;
};
}

#endif