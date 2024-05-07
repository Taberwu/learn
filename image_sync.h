/****************************************************************************************
 * @FilePath: image_sync.h
 * @Author: Taber.wu
 * @Date: 2024-05-06 17:50:37
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-05-07 17:26:32
 * @Copyright: 2024 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#pragma once

#include <condition_variable>
#include <stdint.h>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <memory>
#include <shared_mutex>
#include <atomic>
#include <stdlib.h>
#include <time.h> 
#include <deque>
#include <iomanip>
#include <functional>

#include <jsos/message/approximate_synchronizer.hpp>

#include "ros/ros.h"
#include "rosbag/bag.h"
#include <sensor_msgs/Imu.h>
#include <vehicle_msgs/EgoVehicleInfo.h>
#include <sensor_msgs/Image.h>
#include <boost/shared_ptr.hpp>

namespace jsos{namespace algorithm{namespace slam{
class ImageSync{
    public:
        ImageSync(ros::NodeHandle& handler, const float use_image_hz, std::uint32_t data_flag = 0U, bool record_bag = false);
        virtual ~ImageSync();
        void syncRecordImu(const ::sensor_msgs::Imu::ConstPtr& message);
        void syncRecordWheel(const vehicle_msgs::EgoVehicleInfo::ConstPtr& message);

        void registerStereoHandler(const std::function<void(std::shared_ptr<::sensor_msgs::Image::ConstPtr>, std::shared_ptr<::sensor_msgs::Image::ConstPtr>)> &stereo_handler){
                stereo_handler_ = stereo_handler;
             }
        void registerSurroundHandler(const std::function<void(std::shared_ptr<::sensor_msgs::Image::ConstPtr>, std::shared_ptr<::sensor_msgs::Image::ConstPtr>,
             std::shared_ptr<::sensor_msgs::Image::ConstPtr>, std::shared_ptr<::sensor_msgs::Image::ConstPtr>)> &surround_handler){
                surround_handler_ = surround_handler;
             }
    
        void registerSixCameraHandler(const std::function<void(std::shared_ptr<::sensor_msgs::Image::ConstPtr>, std::shared_ptr<::sensor_msgs::Image::ConstPtr>, std::shared_ptr<::sensor_msgs::Image::ConstPtr>,
             std::shared_ptr<::sensor_msgs::Image::ConstPtr>, std::shared_ptr<::sensor_msgs::Image::ConstPtr>, std::shared_ptr<::sensor_msgs::Image::ConstPtr>)> &six_camera_handler){
                six_camera_handler_ = six_camera_handler;
             }
    protected:
    template<typename MsgAPtr, typename MsgBPtr, typename...Args>
    std::uint64_t findMaxStamp(MsgAPtr &a, MsgBPtr &b, Args& ...args){
        auto stamp_a = (*a)->header.stamp.toNSec();
        auto stamp_b = findMaxStamp(b, args...);
        std::uint64_t max_stamp = (stamp_a < stamp_b)? stamp_b : stamp_a;
        return max_stamp;
    }
    template<typename MsgAPtr, typename MsgBPtr>
    std::uint64_t findMaxStamp(MsgAPtr &a, MsgBPtr &b){
        auto stamp_a = (*a)->header.stamp.toNSec();
        auto stamp_b = (*b)->header.stamp.toNSec();

        std::uint64_t max_stamp = (stamp_a < stamp_b)? stamp_b : stamp_a;
        return max_stamp;
    }

    template <int idx>
    void syncImageHandler(const ::sensor_msgs::Image::ConstPtr& message){
        if(surr_sync_){
            if(idx > 4) return;
            surr_sync_->insert<idx>(std::make_shared<::sensor_msgs::Image::ConstPtr> (message));
        }
        // if(stereo_sync_){
        //     if(idx < 4) return;
        //     stereo_sync_->insert<(idx-4)>(std::make_shared<::sensor_msgs::Image::ConstPtr> (message));
        // }
        // if(cameras_sync_){
        //     cameras_sync_->insert<idx>(std::make_shared<::sensor_msgs::Image::ConstPtr> (message));
        // }
    }

    std::thread* setRocordImgThread(const int &index, const std::string& topic_name1, const std::string& topic_name2, bool write_bag = true){
        return (new std::thread([&](){
            while(record_data_){
                std::shared_lock<std::shared_mutex> lck(record_swap_lck_);
                condi_.wait(lck);
               
                // auto start_stamp = std::chrono::system_clock::now();
                while(!images_r_dq_[index]->empty()){
                    record_bag_[index].write(topic_name1,  images_r_dq_[index]->front()[0]->header.stamp, images_r_dq_[index]->front()[0]);
                    record_bag_[index].write(topic_name2,  images_r_dq_[index]->front()[1]->header.stamp, images_r_dq_[index]->front()[1]);
                    images_r_dq_[index]->pop_front();
                }
                write_flag_ |= (1 << index);
                // printf("write flag %d\n", write_flag_.load());
                // auto end_stamp = std::chrono::system_clock::now();
                // std::uint64_t run_time = std::chrono::duration<double, std::milli>(end_stamp - start_stamp).count();
                // ROS_INFO("record image %d bag  use %lu", index, run_time);
            }
        }));
    
    }

    private:
        std::shared_ptr<jsos::message::ApproximateSynchronizer<::sensor_msgs::Image::ConstPtr, ::sensor_msgs::Image::ConstPtr>>  stereo_sync_;
        std::shared_ptr<jsos::message::ApproximateSynchronizer<::sensor_msgs::Image::ConstPtr,  
                ::sensor_msgs::Image::ConstPtr, ::sensor_msgs::Image::ConstPtr, ::sensor_msgs::Image::ConstPtr>>  surr_sync_;
        std::shared_ptr<jsos::message::ApproximateSynchronizer<::sensor_msgs::Image::ConstPtr, ::sensor_msgs::Image::ConstPtr, ::sensor_msgs::Image::ConstPtr, 
                ::sensor_msgs::Image::ConstPtr, ::sensor_msgs::Image::ConstPtr, ::sensor_msgs::Image::ConstPtr>>  cameras_sync_;

        std::function<void(std::shared_ptr<::sensor_msgs::Image::ConstPtr>, std::shared_ptr<::sensor_msgs::Image::ConstPtr>)> stereo_handler_;
        std::function<void(std::shared_ptr<::sensor_msgs::Image::ConstPtr>, std::shared_ptr<::sensor_msgs::Image::ConstPtr>,
             std::shared_ptr<::sensor_msgs::Image::ConstPtr>, std::shared_ptr<::sensor_msgs::Image::ConstPtr>)> surround_handler_;
        std::function<void(std::shared_ptr<::sensor_msgs::Image::ConstPtr>, std::shared_ptr<::sensor_msgs::Image::ConstPtr>, std::shared_ptr<::sensor_msgs::Image::ConstPtr>,
             std::shared_ptr<::sensor_msgs::Image::ConstPtr>, std::shared_ptr<::sensor_msgs::Image::ConstPtr>, std::shared_ptr<::sensor_msgs::Image::ConstPtr>)> six_camera_handler_;
        ros::NodeHandle& handler_;
        ros::Subscriber surround_front_subscriber_;
        ros::Subscriber surround_back_subscriber_;
        ros::Subscriber surround_left_subscriber_;
        ros::Subscriber surround_right_subscriber_;
        ros::Subscriber stereo_right_subscriber_;
        ros::Subscriber stereo_left_subscriber_;
        double image_hz_;
        std::string topic_surround_left_;
        std::string topic_surround_right_;
        std::string topic_surround_front_;
        std::string topic_surround_rear_;
        std::string topic_stereo_left_;
        std::string topic_stereo_right_;
        std::uint64_t last_stamp_;
        std::string imagebag_path_;
        std::vector<rosbag::Bag> record_bag_;
        std::shared_ptr<std::deque<::sensor_msgs::Imu::ConstPtr>> imu_w_dq_;
        std::shared_ptr<std::deque<::sensor_msgs::Imu::ConstPtr>> imu_r_dq_;
        std::shared_ptr<std::deque<vehicle_msgs::EgoVehicleInfo::ConstPtr>> wheel_w_dq_;
        std::shared_ptr<std::deque<vehicle_msgs::EgoVehicleInfo::ConstPtr>> wheel_r_dq_;
        std::vector<std::shared_ptr<std::deque<std::vector<::sensor_msgs::Image::ConstPtr>>>> images_w_dq_;
        std::vector<std::shared_ptr<std::deque<std::vector<::sensor_msgs::Image::ConstPtr>>>> images_r_dq_;
        std::shared_mutex record_swap_lck_;

        std::vector<std::shared_ptr<std::thread>> record_threads_;
        std::atomic_bool record_data_;  
        std::atomic_int16_t write_flag_;
        std::condition_variable_any condi_;
        std::int16_t all_msgs_;
        int sur_fb_idx_, sur_lr_idx_, ste_lr_idx_;
        std::uint64_t wheel_cnt_, wheel_w_laststamp_, wheel_r_laststamp_;
        std::uint64_t wheel_record_cnt_;
    };
    
}}}