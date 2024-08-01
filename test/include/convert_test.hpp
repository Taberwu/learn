/****************************************************************************************
 * @FilePath: convert_test.hpp
 * @Author: Taber.wu
 * @Date: 2024-06-14 14:10:53
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-06-14 15:00:55
 * @Copyright: 2024 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef DATA_TRANS_HPP___
#define DATA_TRANS_HPP___

#include <map>
#include <memory>

#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include <Eigen/Core>

#include "rotation.hpp"

using namespace jsos::utility;
namespace convert
{
    struct Imu{
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        public:
        Eigen::Vector3d  linear_acceleration;  
        std::uint64_t stamp;
    };
    struct IntegratedNavigation{
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        public:
        Eigen::Vector3d  rotation;  
        Eigen::Vector3d  position;  
        Eigen::Vector3d  velocity;  
        int sat_type;
        std::uint64_t stamp;
    };

    typedef std::pair<std::shared_ptr<Imu>, std::shared_ptr<Imu>> NearImuData;
    typedef std::pair<std::shared_ptr<IntegratedNavigation>, std::shared_ptr<IntegratedNavigation>> NearInspvaData;
struct CarState{
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    std::uint64_t timestamp;
    std::int32_t status;
    Eigen::Vector3d lla;
    Eigen::Vector3d velocity;
    Eigen::Vector3d acceleration;
    Eigen::Quaterniond rot;
};

class Datatrans{
public:

    std::shared_ptr<CarState> calculateState(const uint64_t stamp, NearImuData& imu,  NearInspvaData& inspva){
    if((!imu.second) ||(!inspva.second))
        return nullptr;
    // if(inspva.second->sat_type.type < 55){
    //     ROS_WARN(" stamp %lu SatelliteType %u not ready", stamp, inspva.second->sat_type.type);
    //     return nullptr;
    // }
    auto status = (inspva.second)->sat_type;
    
    auto car_state = std::make_shared<CarState>();
    car_state->timestamp = stamp;
    if(!imu.first){
        car_state->acceleration = Eigen::Vector3d(imu.second->linear_acceleration.x(), imu.second->linear_acceleration.y(), imu.second->linear_acceleration.z());
    }
    else{
        double k = ((double)stamp - (double)imu.first->stamp)/((double)imu.second->stamp - (double)imu.first->stamp);
        Eigen::Vector3d a_p(imu.first->linear_acceleration.x(), imu.first->linear_acceleration.y(), imu.first->linear_acceleration.z());
        Eigen::Vector3d a_n(imu.second->linear_acceleration.x(), imu.second->linear_acceleration.y(), imu.second->linear_acceleration.z());
        car_state->acceleration = a_p + k * (a_n - a_p);
    }
    if(!inspva.first){
        Eigen::Vector3d euler(inspva.second->rotation.x(), inspva.second->rotation.y(), inspva.second->rotation.z());
        car_state->lla = Eigen::Vector3d(inspva.second->position.x(), inspva.second->position.y(), inspva.second->position.z());
        car_state->velocity = Eigen::Vector3d(inspva.second->velocity.x(), inspva.second->velocity.y(), inspva.second->velocity.z());
        car_state->rot = eulerAngle2Quaternion(euler);
    }
    else{
        if(status > (inspva.first)->sat_type){
            status = (inspva.first)->sat_type;
        }
        double k = ((double)stamp - (double)inspva.first->stamp)/((double)inspva.second->stamp - (double)inspva.first->stamp);
        Eigen::Vector3d euler_p(inspva.first->rotation.x(), inspva.first->rotation.y(), inspva.first->rotation.z());
        Eigen::Vector3d p_p(inspva.first->position.x(), inspva.first->position.y(), inspva.first->position.z());
        Eigen::Vector3d v_p(inspva.first->velocity.x(), inspva.first->velocity.y(), inspva.first->velocity.z());
        Eigen::Vector3d euler_n(inspva.second->rotation.x(), inspva.second->rotation.y(), inspva.second->rotation.z());
        Eigen::Vector3d p_n(inspva.second->position.x(), inspva.second->position.y(), inspva.second->position.z());
        Eigen::Vector3d v_n(inspva.second->velocity.x(), inspva.second->velocity.y(), inspva.second->velocity.z());

        car_state->lla = p_p + k * (p_n -p_p);
        car_state->velocity = v_p + k * (v_n - v_p);
        auto qa =  eulerAngle2Quaternion(euler_p);
        auto qb = eulerAngle2Quaternion(euler_n);
        printf("ea [%.7f %.7f %.7f]\n",  euler_p.x(), euler_p.y(),euler_p.z());
        printf("eb [%.7f %.7f %.7f]\n",  euler_n.x(), euler_n.y(),euler_n.z());
        printf("qa [%.7f %.7f %.7f %.7f]\n", qa.w(), qa.x(), qa.y(),qa.z());
        printf("qb [%.7f %.7f %.7f %.7f]\n", qb.w(), qb.x(), qb.y(),qb.z());
        printf("ins k %.2f \n", k);
         car_state->rot = qa.slerp(k, qb);
        car_state->status = status;
    }
    return car_state;
}
    void test(){
        auto imu_a =std::make_shared<Imu>();
        imu_a->stamp = 0;
        imu_a->linear_acceleration = Eigen::Vector3d::Zero();
        auto imu_b =std::make_shared<Imu>();
        imu_b->stamp = 10;
        imu_b->linear_acceleration = 10. * Eigen::Vector3d::Ones();
        auto imu = std::make_pair(imu_a, imu_b);

        auto ins_a = std::make_shared<IntegratedNavigation>();
        ins_a->stamp = 2;
        ins_a->sat_type = 56;
        ins_a->position = Eigen::Vector3d::Zero();
        ins_a->velocity = -5 * Eigen::Vector3d::Ones();
        ins_a->rotation = Eigen::Vector3d::Zero();

        auto ins_b = std::make_shared<IntegratedNavigation>();
        ins_b->stamp = 6;
        ins_b->sat_type = 55;
        ins_b->position = Eigen::Vector3d(8., -4., 3.);
        ins_b->velocity = 7 * Eigen::Vector3d::Ones();
        ins_b->rotation = Eigen::Vector3d(0.,0., 120./180.*M_PI);

        auto ins = std::make_pair(ins_a, ins_b);

        auto state = calculateState(4, imu, ins);

        printf("%lu %d \npos [%.3f %.3f %.3f] \n v[%.3f %.3f %.3f]\n q[%.7f %.7f %.7f %.7f]\n [%.3f %.3f %.3f]\n",
        state->timestamp, state->status,
        state->lla.x(), state->lla.y(),state->lla.z(),
        state->velocity.x(), state->velocity.y(), state->velocity.z(),
        state->rot.w(),  state->rot.x(),  state->rot.y(),  state->rot.z(),
        state->acceleration.x(),  state->acceleration.y(), state->acceleration.z());
    }

};
} 


#endif