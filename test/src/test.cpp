/**
****************************************************************************************
 * @FilePath: test.cpp
 * @Author: Taber.wu
 * @Date: 2022-10-09 11:24:39
 * @LastEditors: 
 * @LastEditTime: 2022-10-09 11:24:39
 * @Copyright: 2022 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************
*/
#include <condition_variable>
#include <chrono>
#include <deque>
#include <map>
#include <memory>
#include <mutex>
#include <vector>
#include <iostream>
#include <random>
#include <shared_mutex>
#include <thread>
#include <utility>
#include <unistd.h>
#include <variant>
#include <vector>

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>

#include<test3.h>

// class Message{
// public:
//     Message(){}
//     virtual ~Message(){}
// public:
//     void setTimestamp(std::uint64_t timestamp)
//     {
//         timestamp_ = timestamp;
//     }
//     std::uint64_t getTimestamp()
//     {
//         return timestamp_;
//     }
// protected:
//     std::uint64_t timestamp_;
// };

// class LocalizationMessage : public Message{
// public:
//     LocalizationMessage():origin_(Eigen::Vector3d::Zero()),lla_(Eigen::Vector3d::Zero()),
//     translation_(Eigen::Vector3d::Zero()),velocity_(Eigen::Vector3d::Zero()),
//     rotation_(Eigen::Quaterniond::UnitRandom()),acceleration_(Eigen::Vector3d::Zero()),
//     omega_(Eigen::Vector3d::Zero())
//     {}
//     virtual ~LocalizationMessage(){}

//     void setOrigin(const Eigen::Vector3d& origin){origin_ = origin;}
//     const Eigen::Vector3d getOrigin() const {return origin_;}
//     void setLLA(const Eigen::Vector3d& lla){lla_ = lla;}
//     const Eigen::Vector3d getLLA() const {return lla_;}
//     void setTranslation(const Eigen::Vector3d& translation){translation_ = translation;}
//     const Eigen::Vector3d getTranslation() const {return translation_;}
//     void setVelocity(const Eigen::Vector3d& velocity){velocity_ = velocity;}
//     const Eigen::Vector3d getVelocity() const {return velocity_;}
//     void setRotation(const Eigen::Quaterniond& rotation){rotation_ = rotation;}
//     const Eigen::Quaterniond getRotation() const {return rotation_;}
//     void setAcceleration(const Eigen::Vector3d& acceleration){acceleration_ = acceleration;}
//     const Eigen::Vector3d getAcceleration() const{return acceleration_;}
//     void setOmega(const Eigen::Vector3d& omega){omega_ = omega;}
//     const Eigen::Vector3d getOmega() const{return omega_;}

// protected:
//     Eigen::Vector3d origin_;// latitude, longitude, altitude;
//     Eigen::Vector3d lla_;// latitude, longitude, altitude;
//     Eigen::Vector3d translation_;
//     Eigen::Vector3d velocity_;
//     Eigen::Quaterniond rotation_;
//     Eigen::Vector3d acceleration_;
//     Eigen::Vector3d omega_;
// };

// class WheelMessage : public Message{
//     public:
//         WheelMessage() : fr_speed_(0.), fl_speed_(0.), rr_speed_(0.), rl_speed_(0.) {}
//         virtual ~WheelMessage(){}
//     public:
//         void setFRSpeed(const double& fr_speed){fr_speed_ = fr_speed;}
//         const double getFRSpeed() const {return fr_speed_;}
//         void setFLSpeed(const double& fl_speed){fl_speed_ = fl_speed;}
//         const double getFLSpeed() const {return fl_speed_;}
//         void setRRSpeed(const double& rr_speed){rr_speed_ = rr_speed;}
//         const double getRRSpeed() const {return rr_speed_;}
//         void setRLSpeed(const double& rl_speed){rl_speed_ = rl_speed;}
//         const double getRLSpeed() const {return rl_speed_;}
//         void setRRPluse(const uint32_t& wheeledges_rr){wheeledges_rr_ = wheeledges_rr;}
//         void setRLPluse(const uint32_t& wheeledges_rl){wheeledges_rl_ = wheeledges_rl;}
//         std::uint32_t getRPluse() const {return (wheeledges_rr_+wheeledges_rl_)/2;}
//     protected:
//         double fr_speed_;
//         double fl_speed_;
//         double rr_speed_;
//         double rl_speed_;
//         std::uint32_t wheeledges_rr_;
//         std::uint32_t wheeledges_rl_;
//     };

// class InspvaMessage : public Message{
// public:
//     InspvaMessage(){}
//     virtual ~InspvaMessage(){}
// public:
//     void setLLA(const Eigen::Vector3d& lla){lla_ = lla;}
//     const Eigen::Vector3d getLLA() const {return lla_;}
//     void setVelocity(const Eigen::Vector3d& velocity){velocity_ = velocity;}
//     const Eigen::Vector3d getVelocity() const {return velocity_;}
//     void setRotation(const Eigen::Quaterniond& rotation){rotation_ = rotation;}
//     const Eigen::Quaterniond getRotation() const {return rotation_;}
//     void setLLAStd(const Eigen::Vector3d& noise){lla_std_ = noise;}
//     const Eigen::Vector3d getLLAStd() const {return lla_std_;}
//     void setVelocityStd(const Eigen::Vector3d& noise){velocity_std_ = noise;}
//     const Eigen::Vector3d getVelocityStd() const {return velocity_std_;}
//     void setRotationStd(const Eigen::Vector3d& noise){rotation_std_ = noise;}
//     const Eigen::Vector3d getRotationStd() const {return rotation_std_;}
// protected:
//     Eigen::Vector3d lla_;// latitude, longitude, altitude;
//     Eigen::Vector3d velocity_;
//     Eigen::Quaterniond rotation_;
//     Eigen::Vector3d lla_std_;
//     Eigen::Vector3d velocity_std_;
//     Eigen::Vector3d rotation_std_;
// };

// typedef std::variant<std::shared_ptr<Message>,std::shared_ptr<LocalizationMessage>,std::shared_ptr<WheelMessage>,std::shared_ptr<InspvaMessage>> Dataware;
// typedef std::deque<std::shared_ptr<LocalizationMessage>> LocQue;
// typedef std::shared_ptr<LocQue> LocQuePtr;

// class LocalizationQue{
// public:    
//     LocalizationQue():count(0),data_ready_(0){loc_data_ = std::make_shared<LocQue>();}
//     virtual ~LocalizationQue(){}
//     std::thread getInsertThread(const std::shared_ptr<LocalizationMessage> msg,std::condition_variable_any *data_sts,
//         std::mutex *locker, LocQuePtr &loc_data,uint64_t *data_ready){
//             return std::thread(&LocalizationQue::insert_data,this,msg,data_sts,locker,std::ref(loc_data),data_ready);
//     }

// private:
//      void insert_data(const std::shared_ptr<LocalizationMessage> msg,std::condition_variable_any *data_sts,
//     std::mutex *locker,LocQuePtr &loc_data,uint64_t *data_ready){
//         data_sts = &data_sts_;
//         locker = &locker_;
//         std::unique_lock<std::mutex>lock(locker_);
//         loc_data = loc_data_;
//         loc_data_->emplace_back(msg);
//         if(loc_data_->size()>20){
//             data_ready_ = 1;
//             std::cerr<<"LocalizationQue data "<<data_ready_<<std::endl;
//             loc_data_->pop_front();
//             data_sts_.notify_all();
//         }
//         count++;
//         if(count == 25){
//             loc_data_->clear();
//             count = 0;
//             data_ready_ = 0;
//             data_sts_.notify_all();
//             usleep(30000);
            
//         }
//         *data_ready = data_ready_;
//     }

//     LocQuePtr loc_data_;
//     int count;
//     uint64_t data_ready_;
//     std::condition_variable_any data_sts_;
//     std::mutex locker_;
// };

// class LocalizationVec{
// public:
//     LocalizationVec():data_ready_(0),first_(true){loc_data_.clear();}
//     virtual ~LocalizationVec(){}
//     std::thread converThread(std::condition_variable_any &data_sts,std::mutex &lock,
//             LocQuePtr loc_data,uint64_t &data_ready){
//         return std::thread(&LocalizationVec::getdata,this,std::ref(data_sts),std::ref(lock),loc_data,std::ref(data_ready));
//     }
// private:
//     void getdata(std::condition_variable_any &data_sts,std::mutex &locker,
//                 LocQuePtr loc_data,uint64_t &data_ready){
//         if(first_){
//             std::cerr<<"wait"<<std::endl;
//             loc_data_.clear();
//             usleep(190000);
//             first_ = false;
//             return;
//         }
//         else
//         {
//         std::unique_lock<std::mutex> wheel_locker(locker);
//         if(data_sts.wait_for(wheel_locker,std::chrono::milliseconds(20),[&]{
//         return (data_ready  == 1);})){
//             loc_data_.assign(loc_data->begin(),loc_data->end());
//         }
//         else{
//             std::cerr<<"LocalizationVec data "<<data_ready<<std::endl;
//             std::cerr<<"time out"<<std::endl;
//             return;
//         }
//         if(loc_data_.size()>0)
//         std::cerr<<"vector back time "<<loc_data_.back()->getTimestamp()<<std::endl;
//         }
        

//     }
//     std::vector<std::shared_ptr<LocalizationMessage>> loc_data_;
//     uint64_t data_ready_;
//     bool first_;
// };

// template<typename T>
// void getdataque(std::map<int,std::deque<Dataware>> &dateware,const int a,std::deque<T> &dataque){
   
//     if(!std::holds_alternative<T>(dateware[a].front())) return;
//     for(auto &data:dateware[a])
//     dataque.emplace_back(std::get<T>(data));
// }

int main(int argc, char **argv){
    // auto imuMsg = std::make_shared<LocalizationMessage>();
    // auto rtkMsg = std::make_shared<InspvaMessage>();
    // auto wheMsg = std::make_shared<WheelMessage>() ;
    // auto locQue = std::make_shared<LocalizationQue>();
    // auto locVec = std::make_shared<LocalizationVec>();
    // std::shared_ptr<std::map<int,std::deque<Dataware>>> data;

    // std::condition_variable_any data_sts;
    // std::mutex locker;
    // uint64_t data_ready = 0;
    // auto  imu_data = std::make_shared<LocQue>();

    // for(int i = 0;i < 100;i++){
    //    imuMsg->setTimestamp(i*1000*1000);
    //    imuMsg->setRotation(Eigen::Quaterniond::UnitRandom());

    // //    if(i%2==1){
    // //        wheMsg->setTimestamp(i*1000*1000);
    // //        wheMsg->setRLPluse(random()%5);
    // //        wheMsg->setRRPluse(random()%5);
    // //        data[2].emplace_back(wheMsg);
    // //    }
    // //    if(i%3==2){
    // //        rtkMsg->setTimestamp(i*1000*1000);
    // //        rtkMsg->setLLA(Eigen::Vector3d(31.4356-i/1000.0,121.3425+i/1000.0,(random()%50)/25.0));
    // //        data[3].emplace_back(rtkMsg);
    // //    }
    
    // auto threadA = locQue->getInsertThread(imuMsg,&data_sts,&locker,imu_data,&data_ready);
    // auto threadB = locVec->converThread(data_sts,locker,imu_data,data_ready);
    // threadA.join();
    // std::cerr<<"in main data &&size "<<data_ready<<" "<<imu_data->size()<<std::endl;
    // threadB.detach();

    // usleep(10000);
    // }
   std::string filepath = "/home/wyy/Documents";
   auto test = std::make_shared<test::Test3>(filepath);
   test->readtest();
    return 0;
}
