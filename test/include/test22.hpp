/****************************************************************************************
 * @FilePath: test22.hpp
 * @Author: Taber.wu
 * @Date: 2023-08-22 17:01:44
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-08-23 11:33:55
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef _ROTATION_TEST_HPP_
#define _ROTATION_TEST_HPP_
#include <Eigen/Core>
#include <Eigen/Eigen>

namespace rotation{
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
class RotationTest{

};
};

#endif