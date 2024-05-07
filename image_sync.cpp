/****************************************************************************************
 * @FilePath: image_sync.cpp
 * @Author: Taber.wu
 * @Date: 2024-05-07 10:01:39
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-05-07 17:14:58
 * @Copyright: 2024 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#include <jsslam_ros/image_sync.h>

namespace jsos{namespace algorithm{namespace slam{
void ImageSync::syncRecordImu(const ::sensor_msgs::Imu::ConstPtr& message){
    if(!record_data_) return;
    std::shared_lock<std::shared_mutex> lck(record_swap_lck_);
    imu_w_dq_->emplace_back(message);
    if(imu_w_dq_->size() > 1000) {ROS_WARN("imu data record large than 10s");}
}

void ImageSync::syncRecordWheel(const vehicle_msgs::EgoVehicleInfo::ConstPtr& message){
    if(!record_data_) return;
    std::shared_lock<std::shared_mutex> lck(record_swap_lck_);
    auto stamp = message->header.stamp.toNSec();
    int64_t dt = (int64_t)stamp - (int64_t)wheel_w_laststamp_;
    if(fabs(dt) > 3 *1e7){
        ROS_WARN("%lu wheel receive gap too long %ld", stamp, dt);
    }
    wheel_w_laststamp_ = stamp;
    wheel_w_dq_->emplace_back(message);
    if(wheel_w_dq_->size() > 500){
      ROS_WARN("wheel data record large than 10s");
    }
}

ImageSync::~ImageSync(){
    record_data_ = false;
    // printf("record/receive wheel cnt when destory %lu/%lu  --%.5f\n", wheel_record_cnt_, wheel_cnt_, float(wheel_record_cnt_)/float(wheel_cnt_));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    for(size_t i = 0; i < record_bag_.size(); i++){
        record_bag_[i].close();
        printf("close bag %lu\n", i);
    }
    for(auto &record_th : record_threads_){
        if(record_th->joinable()){
            record_th->join();
        }
    }     
}

ImageSync::ImageSync(ros::NodeHandle& handler, const float use_image_hz, std::uint32_t data_flag, bool record_bag):
    handler_(handler), image_hz_(use_image_hz), all_msgs_(data_flag)
{
    std::atomic_init(&record_data_, record_bag);
    cameras_sync_ = nullptr;
    surr_sync_ = nullptr;
    stereo_sync_ = nullptr;
    write_flag_ = all_msgs_;
    topic_stereo_left_ = "";
    topic_stereo_right_ = "";
    topic_surround_front_ = "";
    topic_surround_left_ = "";
    topic_surround_right_ = "";
    topic_surround_rear_ = "";
    imagebag_path_ = "";
    last_stamp_ = 0;
    sur_fb_idx_ = 0, sur_lr_idx_ = 1, ste_lr_idx_ = 2;
    handler_.getParam("topic_stereo_left", topic_stereo_left_);
    handler_.getParam("topic_stereo_right", topic_stereo_right_);
    handler_.getParam("topic_surround_front", topic_surround_front_);
    handler_.getParam("topic_surround_left", topic_surround_left_);
    handler_.getParam("topic_surround_right", topic_surround_right_);
    handler_.getParam("topic_surround_rear", topic_surround_rear_);
    handler_.getParam("imagebag_path",imagebag_path_);

    for(int i = 0; i < 3; i++){
            images_r_dq_.push_back(std::make_shared<std::deque<std::vector<sensor_msgs::Image::ConstPtr>>>()); 
            images_w_dq_.push_back(std::make_shared<std::deque<std::vector<sensor_msgs::Image::ConstPtr>>>());   
    }
    size_t bag_cnt = 0;
    for(int i = 0; i < 15; i++){
        if((data_flag >> i) & (0b01)){
            bag_cnt ++;
        }
    }
    if((bag_cnt-1) == 3){
         cameras_sync_ = std::make_shared<jsos::message::ApproximateSynchronizer<sensor_msgs::Image::ConstPtr, sensor_msgs::Image::ConstPtr
                                ,sensor_msgs::Image::ConstPtr, sensor_msgs::Image::ConstPtr
                                ,sensor_msgs::Image::ConstPtr, sensor_msgs::Image::ConstPtr>>(20000000,
            [&](std::shared_ptr<sensor_msgs::Image::ConstPtr> msg){return (*msg)->header.stamp.toNSec();}, [&](std::shared_ptr<sensor_msgs::Image::ConstPtr> msg){return (*msg)->header.stamp.toNSec();},
            [&](std::shared_ptr<sensor_msgs::Image::ConstPtr> msg){return (*msg)->header.stamp.toNSec();}, [&](std::shared_ptr<sensor_msgs::Image::ConstPtr> msg){return (*msg)->header.stamp.toNSec();},
            [&](std::shared_ptr<sensor_msgs::Image::ConstPtr> msg){return (*msg)->header.stamp.toNSec();}, [&](std::shared_ptr<sensor_msgs::Image::ConstPtr> msg){return (*msg)->header.stamp.toNSec();});
         cameras_sync_->registerHandler(
            [&](std::shared_ptr<sensor_msgs::Image::ConstPtr> m0,  std::shared_ptr<sensor_msgs::Image::ConstPtr> m1 
                ,std::shared_ptr<sensor_msgs::Image::ConstPtr> m2,  std::shared_ptr<sensor_msgs::Image::ConstPtr> m3
                ,std::shared_ptr<sensor_msgs::Image::ConstPtr> m4, std::shared_ptr<sensor_msgs::Image::ConstPtr> m5
                )
            {
                std::uint64_t lastest_stamp = findMaxStamp(m0, m1, m2, m3, m4, m5);
                if((image_hz_ <= 0.) || (last_stamp_ + (1./image_hz_)*1e9 < lastest_stamp)){
                    six_camera_handler_(m0, m1, m2, m3, m4, m5);
                    if(record_data_){
                        std::vector<sensor_msgs::Image::ConstPtr> buffer0, buffer1, buffer2;
                        buffer0.emplace_back(*m0);
                        buffer0.emplace_back(*m1);
                        buffer1.emplace_back(*m2);
                        buffer1.emplace_back(*m3);
                        buffer2.emplace_back(*m4);
                        buffer2.emplace_back(*m5);

                        std::shared_lock<std::shared_mutex> lck(record_swap_lck_);
                        images_w_dq_[sur_fb_idx_]->emplace_back(buffer0);
                        images_w_dq_[sur_lr_idx_]->emplace_back(buffer1);
                        images_w_dq_[ste_lr_idx_]->emplace_back(buffer2);
                        condi_.notify_all();
                    }
                    if((last_stamp_ == 0) || (image_hz_ <= 0.)){
                        last_stamp_ = (*m0)->header.stamp.toNSec();
                    }
                    else{
                        last_stamp_ += (1./image_hz_)*1e9;
                    }
                    
                }       
        });
    }
    else if((bag_cnt-1) == 2){
        surr_sync_ = std::make_shared<jsos::message::ApproximateSynchronizer<sensor_msgs::Image::ConstPtr, sensor_msgs::Image::ConstPtr
                                ,sensor_msgs::Image::ConstPtr, sensor_msgs::Image::ConstPtr>>(20000000,
            [&](std::shared_ptr<sensor_msgs::Image::ConstPtr> msg){return (*msg)->header.stamp.toNSec();}, [&](std::shared_ptr<sensor_msgs::Image::ConstPtr> msg){return (*msg)->header.stamp.toNSec();},
            [&](std::shared_ptr<sensor_msgs::Image::ConstPtr> msg){return (*msg)->header.stamp.toNSec();}, [&](std::shared_ptr<sensor_msgs::Image::ConstPtr> msg){return (*msg)->header.stamp.toNSec();});
        surr_sync_->registerHandler(
            [&](std::shared_ptr<sensor_msgs::Image::ConstPtr> m0,  std::shared_ptr<sensor_msgs::Image::ConstPtr> m1, 
                std::shared_ptr<sensor_msgs::Image::ConstPtr> m2,  std::shared_ptr<sensor_msgs::Image::ConstPtr> m3
                )
            {
                std::uint64_t lastest_stamp = findMaxStamp(m0, m1, m2, m3);
                if((image_hz_ <= 0.) || (last_stamp_ + (1./image_hz_)*1e9 < lastest_stamp)){
                    surround_handler_(m0, m1, m2, m3);
                    if(record_data_){
                        std::vector<sensor_msgs::Image::ConstPtr> buffer0, buffer1;
                        buffer0.emplace_back(*m0);
                        buffer0.emplace_back(*m1);
                        buffer1.emplace_back(*m2);
                        buffer1.emplace_back(*m3);
                        std::shared_lock<std::shared_mutex> lck(record_swap_lck_);
                        images_w_dq_[sur_fb_idx_]->emplace_back(buffer0);
                        images_w_dq_[sur_lr_idx_]->emplace_back(buffer1); 
                        condi_.notify_all();
                    }  
                    if((last_stamp_ == 0) || (image_hz_ <= 0.)){
                              last_stamp_ = (*m0)->header.stamp.toNSec();
                           }
                    else{
                        last_stamp_ += (1./image_hz_)*1e9;
                    }
                }       
        });
    }
    else if((bag_cnt-1) == 1){
        stereo_sync_ = std::make_shared<jsos::message::ApproximateSynchronizer<sensor_msgs::Image::ConstPtr, sensor_msgs::Image::ConstPtr>>(20000000,
            [&](std::shared_ptr<sensor_msgs::Image::ConstPtr> msg){return (*msg)->header.stamp.toNSec();}, [&](std::shared_ptr<sensor_msgs::Image::ConstPtr> msg){return (*msg)->header.stamp.toNSec();}
        );
        stereo_sync_->registerHandler([&](std::shared_ptr<sensor_msgs::Image::ConstPtr> m0,  std::shared_ptr<sensor_msgs::Image::ConstPtr> m1){
            std::uint64_t lastest_stamp = findMaxStamp(m0, m1);
             if((image_hz_ <= 0.) || (last_stamp_ + (1./image_hz_)*1e9 < lastest_stamp)){
                stereo_handler_(m0, m1);
                if(record_data_){
                    std::vector<sensor_msgs::Image::ConstPtr> buffer0;
                    buffer0.emplace_back(*m0);
                    buffer0.emplace_back(*m1);
                    std::shared_lock<std::shared_mutex> lck(record_swap_lck_);
                    images_w_dq_[ste_lr_idx_]->emplace_back(buffer0);
                    condi_.notify_all();
                }
                if((last_stamp_ == 0) || (image_hz_ <= 0.)){
                    last_stamp_ = (*m0)->header.stamp.toNSec();
                }
                else{
                    last_stamp_ += (1./image_hz_)*1e9;
                }
             }
        });
    }


    if((record_data_) && !(imagebag_path_.empty())){
        if(!imu_r_dq_)  imu_r_dq_ = std::make_shared<std::deque<sensor_msgs::Imu::ConstPtr>>();
        if(!imu_w_dq_)  imu_w_dq_ = std::make_shared<std::deque<sensor_msgs::Imu::ConstPtr>>();
        if(!wheel_r_dq_) wheel_r_dq_ = std::make_shared<std::deque<vehicle_msgs::EgoVehicleInfo::ConstPtr>>();
        if(!wheel_w_dq_) wheel_w_dq_ = std::make_shared<std::deque<vehicle_msgs::EgoVehicleInfo::ConstPtr>>();

        time_t now = time(0);
        tm *ltm = localtime(&now);
        std::string tmp;
        std::string name = std::to_string((ltm->tm_year) + 1900);name += "-";
        tmp = std::to_string(ltm->tm_mon + 1); if(tmp.length() < 2) tmp = "0" + tmp; name += tmp; name += "-"; 
        tmp = std::to_string(ltm->tm_mday); if(tmp.length() < 2) tmp = "0" + tmp; name += tmp; name += "-";
        tmp = std::to_string(ltm->tm_hour); if(tmp.length() < 2) tmp = "0" + tmp; name += tmp; name += "-";
        tmp = std::to_string(ltm->tm_min);  if(tmp.length() < 2) tmp = "0" + tmp; name += tmp; name += "-";
        tmp = std::to_string(ltm->tm_sec);  if(tmp.length() < 2) tmp = "0" + tmp; name += tmp; 
        
        for(size_t pos = 0; pos < bag_cnt; pos++){
            std::string filename  = imagebag_path_ + name;
            filename += "_"; filename += std::to_string(pos);
            filename += std::string(".bag");
            rosbag::Bag bag;
            bag.open(filename, rosbag::BagMode::Write);
            record_bag_.emplace_back(std::move(bag));
        }
        
        record_threads_.emplace_back(std::shared_ptr<std::thread>(new std::thread([&](){
            while(record_data_.load()){
                std::unique_lock<std::shared_mutex>lck(record_swap_lck_);
                condi_.wait(lck, [&](){return (write_flag_ == all_msgs_);});
                std::swap(imu_r_dq_, imu_w_dq_);
                std::swap(wheel_r_dq_, wheel_w_dq_);
                for(size_t pos = 0; pos < images_r_dq_.size(); pos ++){
                    std::swap(images_r_dq_[pos], images_w_dq_[pos]);
                }
                write_flag_ = 0;
            }
        })));
        record_threads_.emplace_back(std::shared_ptr<std::thread>(new std::thread([&](){
            while(record_data_.load()){
               std::shared_lock<std::shared_mutex> lck(record_swap_lck_);
               condi_.wait(lck);
                // auto start_stamp = std::chrono::system_clock::now();
                while(!imu_r_dq_->empty()){
                    record_bag_[3].write("/sensor_msgs/novatel/imu", imu_r_dq_->front()->header.stamp, imu_r_dq_->front());
                    imu_r_dq_->pop_front();
                }
                while(!wheel_r_dq_->empty()){
                    record_bag_[3].write("/canbus/EgoVehicleInfo", wheel_r_dq_->front()->header.stamp, wheel_r_dq_->front());
                    wheel_record_cnt_ ++;
                    auto stamp = wheel_r_dq_->front()->header.stamp.toNSec();
                    int64_t dt = (int64_t)stamp - (int64_t)wheel_r_laststamp_;
                    if(fabs(dt) > 3e7){
                        ROS_WARN("%lu record wheel gap to long %ld", stamp, dt);
                    }
                    wheel_r_laststamp_ = stamp;
                    wheel_r_dq_->pop_front();
                }
                write_flag_ |= 0b1000;
                // auto end_stamp = std::chrono::system_clock::now();
                // std::uint64_t run_time = std::chrono::duration<double, std::milli>(end_stamp - start_stamp).count();
                // ROS_INFO("record wheel+imu bag use %lu", run_time);
                }
        })));
        if((!topic_surround_front_.empty()) && (!topic_surround_rear_.empty()) &&(data_flag &(0b01))){
            record_threads_.emplace_back(std::shared_ptr<std::thread>(setRocordImgThread(sur_fb_idx_, topic_surround_front_, topic_surround_rear_)));
        }
        if((!topic_surround_left_.empty()) && (!topic_surround_right_.empty()) &&((data_flag >> 1) &(0b01))){
            record_threads_.emplace_back(std::shared_ptr<std::thread>(setRocordImgThread(sur_lr_idx_, topic_surround_left_, topic_surround_right_)));
        }
        if((!topic_stereo_left_.empty()) && (!topic_stereo_right_.empty()) &&((data_flag >> 2) &(0b01))){
            record_threads_.emplace_back(std::shared_ptr<std::thread>(setRocordImgThread(ste_lr_idx_, topic_stereo_left_, topic_stereo_right_)));
        }
    }
    if(!topic_surround_front_.empty()){
        surround_front_subscriber_ = handler_.subscribe(topic_surround_front_, 100, &ImageSync::syncImageHandler<0>, this);
    }
    if(!topic_surround_rear_.empty()){
        surround_back_subscriber_ = handler_.subscribe(topic_surround_rear_, 100, &ImageSync::syncImageHandler<1>, this);
    }
    if(!topic_surround_left_.empty()){
        surround_left_subscriber_ = handler_.subscribe(topic_surround_left_, 100, &ImageSync::syncImageHandler<2>, this);
    }
     if(!topic_surround_right_.empty()){
        surround_right_subscriber_ = handler_.subscribe(topic_surround_right_, 100, &ImageSync::syncImageHandler<3>, this);
    }
    // if(!topic_stereo_left_.empty()){
    //     stereo_left_subscriber_ = handler_.subscribe(topic_stereo_left_, 100, &ImageSync::syncImageHandler<4>, this);
    // }
    // if(!topic_stereo_right_.empty()){
    //     stereo_right_subscriber_ = handler_.subscribe(topic_stereo_right_, 100, &ImageSync::syncImageHandler<5>, this);
    // }
}
}}}