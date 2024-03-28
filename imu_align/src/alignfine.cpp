/****************************************************************************************
 * @FilePath: alignfine.cpp
 * @Author: Taber.wu
 * @Date: 2023-10-19 11:39:53
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-12-27 11:11:01
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
   
    Eigen::Vector3d acce_mean[2], gyro_mean[2];
    size_t init_data_num[2] = {0};
    std::uint64_t start_stamp = 0U, init_period = 2000000000;
    acce_mean[0].setZero(); gyro_mean[0].setZero();
    acce_mean[1].setZero(); gyro_mean[1].setZero();
    int k = 0;
    for(auto &rawdata: data){
        if(start_stamp == 0){
            start_stamp = rawdata->timestamp;
        }
        if(rawdata->timestamp > start_stamp + init_period){
            start_stamp = rawdata->timestamp;
            k++;
        }
        if(k> 1) break;
        acce_mean[k] += rawdata->acce;
        gyro_mean[k] += rawdata->gyro;
        init_data_num[k] ++;
        
    }
    for(int t = 0; t<2; t++){
        printf("NO.%d  size %lu [%.7f %.7f %.7f]\n", t, init_data_num[t], acce_mean[t].x(), acce_mean[t].y(), acce_mean[t].z());
        acce_mean[t] = (1./init_data_num[t]) * acce_mean[t];

        gyro_mean[t] = (1./init_data_num[t]) * gyro_mean[t]; 
    }
    
    //  acce_mean[0] = Eigen::Vector3d(-3.98406489, 0., 8.947335651);
    //  acce_mean[1] = Eigen::Vector3d(-3.983110032, 0., 8.947335651);


    printf("imu mean   [%.7f %.7f %.7f] [%.7f %.7f %.7f]\n",
    acce_mean[0].x(), acce_mean[0].y(), acce_mean[0].z(),
    acce_mean[1].x(), acce_mean[1].y(), acce_mean[1].z());

    auto earth_w = GeographicLib::Constants::WGS84_omega<double>();
    auto llr = Eigen::Vector2d(origin_lla_.x()*180./M_PI, origin_lla_.y()*180./M_PI);
    double earth_omega = 1e-9 * earth_w * init_period;
    Eigen::Vector3d grvity_vec = gravity_.array() * Eigen::Array3d(sin(earth_omega)*cos(llr.x()), (1-cos(earth_omega))*sin(llr.x())*cos(llr.x()), 1-(1-cos(earth_omega))*cos(llr.x())*cos(llr.x()));
    Eigen::Matrix3d g_mat, constuct_mat;
    g_mat << grvity_vec.normalized().transpose(),
            Eigen::Vector3d::Zero().transpose(),
            Eigen::Vector3d::Zero().transpose();
    std::cout<<"gravity mat "<<g_mat<<std::endl;
    Eigen::Vector3d constrcut_vec_a = acce_mean[0];
    constrcut_vec_a.normalize();

    Eigen::Vector3d constrcut_vec_b = manif::skew(constrcut_vec_a) * (acce_mean[1]);
    constrcut_vec_b.normalize();
    Eigen::Vector3d constrcut_vec_c = manif::skew(constrcut_vec_b) * (acce_mean[0]);
    constrcut_vec_c.normalize();
    constuct_mat << constrcut_vec_a.transpose(),
                    constrcut_vec_b.transpose(),
                    constrcut_vec_c.transpose();
    std::cout<<"construct mat  "<<constuct_mat<<std::endl;
    rot = Eigen::Quaterniond(g_mat * constuct_mat.inverse());
    euler = jsos::utility::quaternion2EeulerAngle(rot) * 2.;
    euler = 180./M_PI * euler;
}

}