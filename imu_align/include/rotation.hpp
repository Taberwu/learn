#pragma once
#include <cmath>
#include <limits>
#include <Eigen/Eigen>
#include <manif/manif.h>

namespace jsos{namespace utility{

template <typename scalar>
static scalar deg2rad(scalar deg)
{
    return deg * M_PI/180.;
}

template <typename scalar>
static scalar rad2deg(scalar rad)
{
    return rad * 180./M_PI;
}

// constrain the value to [begin, end) range
template<typename T>
static inline T constrain(T begin, T end, T v)
{
    if (begin > end) {
        return constrain(end, begin, v);
    }
    if (v < end && v >= begin) {
        return v;
    }
    const T size = end - begin;// -(std::numeric_limits<T>::epsilon)();
    if (size < (std::numeric_limits<T>::epsilon)()) {
        return v;
    }
    T result = (T)(begin + (T)std::fmod((T)((T)(std::fmod(v - begin, size)) + size), size));
    return result;
}

template<>
inline int constrain(int begin, int end, int v)
{
    if (begin > end) {
        return constrain(end, begin, v);
    }
    if (v < end && v >= begin) {
        return v;
    }
    const int size = end - begin;
    if (size < (std::numeric_limits<int>::epsilon)()) {
        return v;
    }
    int result = begin + (((v - begin) % size) + size) % size;
    return result;
}

static inline float constraitRadZeroTo2Pi(float v)
{
    return constrain(0.f, float(2 * M_PI), v);
}

static inline float constraitRadNegativePiToPi(float v)
{
    return constrain(float(-M_PI), float(M_PI), v);
}

static inline double constraitRadZeroTo2Pi(double v)
{
    return constrain(0., 2 * M_PI, v);
}

static inline double constraitRadNegativePiToPi(double v)
{
    return constrain(-M_PI, M_PI, v);
}

static inline float angleDistanceNegativePiToPi(float dest, float src)
{
    return constraitRadNegativePiToPi(dest - src);
}
static inline double angleDistanceNegativePiToPi(double dest, double src)
{
    return constraitRadNegativePiToPi(dest - src);
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
static Eigen::Matrix<scalar, 3, 3> angleAxis2Rotation(const Eigen::Matrix<scalar, 3, 1>& angleaxis)
{
    scalar theta = angleaxis.norm();
    if(theta < std::numeric_limits<scalar>::epsilon()){
        return Eigen::Matrix<scalar, 3, 3>::Identity();
    }
    Eigen::Matrix<scalar, 3, 1> axis = angleaxis / theta;
    scalar tsin = std::sin(theta);
    scalar tcos = std::cos(theta);
    Eigen::Matrix<scalar, 3, 3> r = Eigen::Matrix<scalar, 3, 3>::Identity() * tcos + (1. - tcos) * axis * axis.transpose() + tsin * manif::skew(axis);
    return r;
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
 static Eigen::Matrix<scalar, 3, 1> rotation2EeulerAngle(const Eigen::Matrix<scalar, 3, 3>& rotation) {
    const Eigen::Matrix<scalar, 3, 1>& n = rotation.col(0);
    const Eigen::Matrix<scalar, 3, 1>& o = rotation.col(1);
    const Eigen::Matrix<scalar, 3, 1>& a = rotation.col(2);

    Eigen::Matrix<scalar, 3, 1> ypr(3);
    scalar y = std::atan2(n(1), n(0));
    scalar p = std::atan2(-n(2), n(0) * std::cos(y) + n(1) * std::sin(y));
    scalar r = std::atan2(a(0) * std::sin(y) - a(1) * std::cos(y), -o(0) * std::sin(y) + o(1) * std::cos(y));
    return Eigen::Matrix<scalar, 3, 1>(r, p, y);
}

}}

