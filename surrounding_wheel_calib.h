/****************************************************************************************
 * @FilePath: surrounding_wheel_calib.h
 * @Author: Taber.wu
 * @Date: 2024-03-19 09:06:29
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-04-26 17:06:27
 * @Copyright: 2024 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#pragma once
#include <deque>
#include <string>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <stdint.h>

#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include <ceres/ceres.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include <camodocal/camera_models/Camera.h>
#include <camodocal/camera_models/EquidistantCamera.h>
#include <camodocal/camera_models/CameraFactory.h>

#include <jsos/utility/rotation.hpp>
#include <jsslam/functional/surround_image_detection.h>
#include <jsslam/algorithm/nn_indexing.h>
#include <jsslam/utility/visual.h>

namespace jsos{namespace algorithm{

class SurroundingWheelCalib{
public:

class SlotCenterFactor{
public:
    SlotCenterFactor(){
        memset(&factor, 0, 5*sizeof(float));
    }
    SlotCenterFactor(float *data){
        memcpy(&factor, data, 5*sizeof(float));
    }
    SlotCenterFactor(const float x, const float y, const float cosx, const float sinx, const float width){
        factor[0] = x;
        factor[1] = y;
        factor[2] = cosx;
        factor[3] = sinx;
        factor[4] = width;
    }
    SlotCenterFactor(Eigen::Matrix<float, 5, 1>& matrix){
        memcpy(&factor, matrix.data(), 5*sizeof(float));
    }
    SlotCenterFactor(const SlotCenterFactor &other){
        memcpy(&factor, other.factor, 5*sizeof(float));
    }
    const SlotCenterFactor& operator= (const SlotCenterFactor &other){
        memcpy(this->factor, other.factor, 5*sizeof(float));
        return *this;
    }

    bool operator== (const SlotCenterFactor& other) const{
        for(int i = 0; i < 5; i++){
            if(this->factor[i] != other.factor[i]){
                return false;
            }
        }
        return true;
    }
    const float x()const {return factor[0];}
    const float y()const {return factor[1];}
    const float cosx()const {return factor[2];}
    const float sinx()const {return factor[3];}
    const float width()const {return factor[4];}
    
    float* data(){return factor;}
    float factor[5];
};

using SlotsCenter = std::map<int, SlotCenterFactor>;//{id, (x, y, cos, sin, width)}
struct ParkingSlotCalib{
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    bool enable;
    std::uint64_t timestamp;
    Eigen::Vector3d tcw;
    Eigen::Quaterniond rcw;
    SlotsCenter slots_center;
    SlotsCenter predict_slots_center;
    cv::Mat image;
    ParkingSlotCalib(){
        enable = true;
        timestamp = 0;
        slots_center.clear();
        predict_slots_center.clear();
        tcw = Eigen::Vector3d::Zero();
        rcw = Eigen::Quaterniond::Identity();
    }
} ;

struct WheelCalib{
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    std::uint64_t timestamp;
    Eigen::Vector3d wheel;
    Eigen::Vector3d tcw;
    Eigen::Quaterniond rcw;
};
public:
    SurroundingWheelCalib(const double& wheel_base, float avm_resize, bool manual_slot = false);
    ~SurroundingWheelCalib() = default;
public:
    void clear(){slot_calibers_.clear(); wheel_calibers_.clear();}
    void insert(const std::shared_ptr<sensor_msgs::Objects>& objects);
    void insert(const std::shared_ptr<sensor_msgs::Wheel>& wheel);
    void remove(const std::vector<size_t>& idx);
    void remove();
    void extract();
    void calib();
    std::string output();
    void getTimestamps(std::vector<std::uint64_t> &timestamps);
    std::deque<std::shared_ptr<WheelCalib>> getWheels(){return wheel_calibers_;}
    std::deque<std::shared_ptr<ParkingSlotCalib>> getParkSlots(){return slot_calibers_;}
    std::shared_ptr<ParkingSlotCalib> getParkSlot(const int index){
        if((index < 0) || index > slot_calibers_.size() -1){ return nullptr;}
        return slot_calibers_[index];
    }
    void draw(int index, cv::Mat& image);
    std::shared_ptr<std::vector<Eigen::Vector3d>> showCalibRes(std::vector<Eigen::Vector3d>& wheel_tra, std::vector<Eigen::Vector3d>& slot_tra);
    void readManualSlot(const std::string &filename);
    void readImage(const std::shared_ptr<sensor_msgs::Objects>& objects);
    void getExternic(Eigen::Vector3d& ex_trans, Eigen::Quaterniond& ex_rot){
        ex_trans = ext_trans_;
        ex_rot = ext_rot_;
    }
protected:
    void sync(std::deque<std::shared_ptr<ParkingSlotCalib>> &slot_calibers,  std::deque<std::shared_ptr<WheelCalib>> &wheel_calibers);
    bool estimateSlotPos(const std::shared_ptr<ParkingSlotCalib>& last_slot, std::shared_ptr<ParkingSlotCalib>& curr_slot, 
            std::shared_ptr<WheelCalib> last_wheel = nullptr, std::shared_ptr<WheelCalib> wheel = nullptr, bool usewheel_predict = false);
    void updateWheelPos(const double vx, const double vw, const double dt, Eigen::Vector3d &t, Eigen::Quaterniond &r);
    void predictSlotPoints(const std::shared_ptr<ParkingSlotCalib>& slot, const Eigen::Quaterniond &to_cur_q, 
                const Eigen::Vector3d &to_cur_t,  SlotsCenter &p_slot_pts);
    void drawSlot(const SlotsCenter& slot_centers, const cv::Scalar color, cv::Mat& image, float depth = 4.1);
    class SurrWheelCalibCostFunction{
        public:
        SurrWheelCalibCostFunction(const Eigen::Vector3d &wheel_t, const Eigen::Quaterniond& wheel_r, const Eigen::Vector3d &sur_t, const Eigen::Quaterniond& sur_r, bool constrait_2d = true) :
            wheel_t_(wheel_t), wheel_r_(wheel_r), sur_t_(sur_t), sur_r_(sur_r), constrait_2d_(constrait_2d)
        {}
        ~SurrWheelCalibCostFunction() = default;

        template<class T>
        T quat2Yaw(const Eigen::Quaternion<T>& q) const
        {
            T siny_cosp = T(2) * (q.w() * q.z() + q.x() * q.y());
            T cosy_cosp = T(1) - T(2) * (q.y() * q.y() + q.z() * q.z());
            T yaw = ceres::atan2(siny_cosp, cosy_cosp);
            return yaw;
        }

        template<class T>
        T normalizeYaw(T yaw) const
        {
            T result;
            while(yaw > T(M_PI)){
               result = yaw - T(2. * M_PI);
            }
            while(yaw < T(-M_PI)){
               result = yaw + T(2. * M_PI);
            }
            return result;
        }

        template <typename T>
        bool operator()(const T* const ext,  T* residuals)const{
            if(constrait_2d_){
                Eigen::Quaternion<T> ext_q(ceres::cos((T)0.5 * ext[2]), (T)0., (T)0., ceres::sin((T)0.5 * ext[2]));
                Eigen::Quaternion<T> wheel_q((T)wheel_r_.w(), (T)wheel_r_.x(), (T)wheel_r_.y(), (T)wheel_r_.z());
                Eigen::Quaternion<T> sur_q((T)sur_r_.w(), (T)sur_r_.x(), (T)sur_r_.y(), (T)sur_r_.z());
                Eigen::Matrix<T, 3, 1> t_w_e((T)wheel_t_.x(), (T)wheel_t_.y(), (T)0.);
                Eigen::Matrix<T, 3, 1> ext_t(ext[0], ext[1], (T)0.);
                Eigen::Matrix<T, 3, 1> t_s((T)sur_t_.x(), (T)sur_t_.y(), (T)0.);
                //p = (ext_t + ext_q.conjugate() *t_s) - (wheel_q.conjugate() * ext_t);
                Eigen::Matrix<T, 3, 1> p1 =  ext_q.conjugate() *t_s;
                p1 =  p1 + ext_t;
                Eigen::Matrix<T, 3, 1> l = wheel_q.conjugate() * ext_t;
                Eigen::Matrix<T, 3, 1> p = p1 -l;


                //q = ext_q * ((t_w_e + sur_q.conjugate() * ext_t) - ext_t) 
                Eigen::Matrix<T, 3, 1> q = sur_q.conjugate() * ext_t;
                q = t_w_e + q - ext_t;
                q = ext_q * q;

                residuals[0] = t_s.x() - q.x();
                residuals[1] = t_s.y() - q.y();
                residuals[2] = t_w_e.x() - p.x();
                residuals[3] = t_w_e.y() - p.y();

            }
            return true;
        }

        static ceres::CostFunction* generateCostFunction(const Eigen::Vector3d &wheel_t, const Eigen::Quaterniond& wheel_r, const Eigen::Vector3d &sur_t, const Eigen::Quaterniond& sur_r){
            return (new ceres::AutoDiffCostFunction<SurrWheelCalibCostFunction, 4, 2>(new SurrWheelCalibCostFunction(wheel_t, wheel_r, sur_t, sur_r, true)));
        }
        private:
           Eigen::Vector3d wheel_t_;
           Eigen::Quaterniond wheel_r_;
           Eigen::Vector3d sur_t_;
           Eigen::Quaterniond sur_r_;
           bool constrait_2d_; 
    };


    class SlotMatchCostFunction{
        public:
        SlotMatchCostFunction(const SlotCenterFactor &cur_point, const SlotCenterFactor &last_point) :
            cur_point_(cur_point), last_point_(last_point)
        {}
        ~SlotMatchCostFunction() = default;

        template <typename T>
        bool operator()(const T* const ext,  T* residuals)const{
                Eigen::Quaternion<T> q (ceres::cos(T(0.5)* ext[0]), T(0.), T(0.),  ceres::sin(T(0.5) * ext[0]));
                Eigen::Matrix<T, 3, 1> ma_ext(ext[1], ext[2], T(0.));
                Eigen::Matrix<T, 3, 1> ma_p_m((T)last_point_.x(), (T)last_point_.y(), T(0.));
                Eigen::Matrix<T, 3, 1> ma_p_e((T)(last_point_.x() + last_point_.cosx()), (T)(last_point_.y() + last_point_.sinx()), T(0.));
                ma_p_m = q * ma_p_m + ma_ext;
                ma_p_e = q * ma_p_e + ma_ext;
                T theta = ceres::atan2(ma_p_e.y() - ma_p_m.y(), ma_p_e.x() - ma_p_m.x());
                residuals[0] = static_cast<T>(cur_point_.x()) - ma_p_m.x();
                residuals[1] = static_cast<T>(cur_point_.y()) - ma_p_m.y();
                residuals[2] = static_cast<T>(cur_point_.cosx()) - ceres::cos(theta);
                residuals[3] = static_cast<T>(cur_point_.sinx()) - ceres::sin(theta);
            return true;
        }

        static ceres::CostFunction* generateCostFunction(const SlotCenterFactor &cur_point, const SlotCenterFactor &last_point){
            return (new ceres::AutoDiffCostFunction<SlotMatchCostFunction, 4, 3>(new SlotMatchCostFunction(cur_point, last_point)));
        }
        private:
       SlotCenterFactor cur_point_;
       SlotCenterFactor last_point_;
    };

private:
    double wheel_base_;
    float avm_resize_;
    bool calib_finished_;
    bool manual_slot_;
    std::deque<std::shared_ptr<ParkingSlotCalib>> slot_calibers_;
    std::deque<std::shared_ptr<WheelCalib>> wheel_calibers_;
    std::shared_ptr<NearestNeighborsIndexing> nn_search_;
    std::shared_ptr<std::vector<Eigen::Vector3d>> calib_err_;
    std::stringstream out_;

    Eigen::Vector3d ext_trans_;
    Eigen::Quaterniond ext_rot_;
    std::uint64_t dbg_last_stamp_;
    std::uint64_t slot_image_cnt_;
};
}}