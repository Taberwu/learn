/****************************************************************************************
 * @FilePath: alignfine.cpp
 * @Author: Taber.wu
 * @Date: 2023-10-19 11:39:53
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-10-27 17:50:13
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#include "alignfine.h"

namespace jsos{

Alignment::Alignment(const Eigen::Vector3d &origin_lla): origin_lla_(origin_lla)
{
    alignment_datas_.clear();
    GeographicLib::GravityModel gravity_model("wgs84");
    gravity_model.Gravity(origin_lla_.x(), origin_lla_.y(), origin_lla_.z(), 
            gravity_.x(), gravity_.y(), gravity_.z());
    gravity_.z() *= -1.;
    printf("gravity %.7f %.7f %.7f\n", gravity_.x(), gravity_.y(), gravity_.z());
}

void Alignment::coarseAlign(const std::deque<std::shared_ptr<ImuData>> &data, Eigen::Vector3d &euler,
                    Eigen::Quaterniond &rot){
   
    Eigen::Vector3d acce_mean, gyro_mean;
    acce_mean.setZero(); gyro_mean.setZero();
    for(auto &rawdata: data){
        acce_mean += rawdata->acce;
        gyro_mean += rawdata->gyro;
    }
    acce_mean = (1./data.size()) * acce_mean;
    gyro_mean = (1./data.size()) * gyro_mean; 
    acce_mean.x() = 6.916966702;
    acce_mean.y() = 0.4596698;
    acce_mean.z() = -6.916966702;

    printf("back data   [%.7f %.7f %.7f] [%.7f %.7f %.7f]\n",
    acce_mean.x(), acce_mean.y(), acce_mean.z(),
    gyro_mean.x(), gyro_mean.y(), gyro_mean.z());


    auto earth_w = GeographicLib::Constants::WGS84_omega<double>();
    auto llr = Eigen::Vector2d(origin_lla_.x()*180./M_PI, origin_lla_.y()*180./M_PI);
    Eigen::Vector3d earth_omega = Eigen::Vector3d(0., earth_w * cos(llr.x()), earth_w * sin(llr.x()));
    auto rot_acc = gravity_.z() * earth_omega.y()-gravity_.y()*earth_omega.z();
    Eigen::Matrix3d g_mat, constuct_mat;
    g_mat << 0., -1.*gravity_.y(), -1.*gravity_.z(),
            rot_acc, 0., 0.,
            0, rot_acc * gravity_.z(), -1.* rot_acc * gravity_.y();

    Eigen::Vector3d constrcut_vec_a = manif::skew(acce_mean) * gyro_mean;
    Eigen::Vector3d constrcut_vec_b = manif::skew(constrcut_vec_a) * acce_mean;
    constuct_mat << gyro_mean, constrcut_vec_a, constrcut_vec_b;

    rot = Eigen::Quaterniond(g_mat.inverse() * constuct_mat);
    euler = jsos::utility::quaternion2EeulerAngle(rot);
    euler = 180./M_PI * euler;
}

}