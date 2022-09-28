/*
 * @Author: wuyuyang Taber.wu@joyson.cn
 * @Date: 2022-09-15 14:40:32
 * @LastEditors: wuyuyang Taber.wu@joyson.cn
 * @LastEditTime: 2022-09-15 16:51:41
 * @FilePath: /test/src/test.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include<memory>
#include<thread>
#include<mutex>
#include<vector>
#include <deque>
#include <chrono>
#include <iostream>
#include <eigen3/Eigen/Core>


using namespace std::chrono_literals;

class MessageRecTest{

    public:
    MessageRecTest(){};

    template <typename Msg>
    void ReceiveMsg(){

    }
   
    template <typename Msg>
    void get_data(std::shared_ptr<Msg>&data){
        std::unique_lock<std::mutex>lock(data_lock_);
        data=dynamic_cast<std::shared_ptr<Msg>>(data_);
    }
    private:
    std::shared_ptr<void>data_;
    std::mutex data_lock_;
  

     template <typename Msg>
    void message_callback(std::shared_ptr<Msg>msg_ptr){
        {
            if(!msg_ptr.get()) return;
            std::unique_lock<std::mutex>lock(data_lock_);
            
            data_.reset(msg_ptr);
        }

}

};

class MessageSendTest{
    public:


};

int main(int argc,char **argv){
    // std::deque<std::thread>threads;
    // threads.push_back(std::thread (&[]{std::this_thread::sleep_for(5s);std::cout<<"thread 1"<<std::endl;}));
    // threads.push_back(std::thread (&[]{std::this_thread::sleep_for(2s);std::cout<<"thread 2"<<std::endl;}));
    // threads.push_back(std::thread ([&]{std::this_thread::sleep_for(3s);std::cout<<"thread 3"<<std::endl;}));

    // for(auto &sub_thread:threads){
    //     sub_thread.join();
    // }

    {
         // if(!IMUdata_que_.empty()) 
    // {
    //     if((message->getTimestamp()<IMUdata_que_.back()->getTimestamp())||
    //     (message->getTimestamp()-IMUdata_que_.back()->getTimestamp() > 5*options_->imu_period)) 
    //     {
    //         IMUdata_que_.clear();
    //         data_ready_ &= ~0b01;
    //     }
    //     else{ 
    //         if(message->getTimestamp()-IMUdata_que_.back()->getTimestamp()<options_->imu_period/10) 
    //         return;
    //     }
      
    // }
    // {std::unique_lock<std::shared_mutex> locker(locker_);
    // IMUdata_que_.emplace_back(message);       
    // if(IMUdata_que_.size()>calibration_interval_/options_->imu_period){
    //     data_ready_ |= 0b01;
    //     IMUdata_que_.pop_front();
    //     data_sts_.notify_all();
    // }
    // }
    }

    Eigen::Vector3d va(1,0,0);
    Eigen::Vector3d vb(0,1,1);
    Eigen::Vector3d vc(1,1,1);

    auto tmp1=va.cross(vb);
    auto tmp2=va.cross(Eigen::Vector3d(1,0,0));
    auto tmp3= va.adjoint()*vb;
    auto tmp4=vc-va;
    auto tmp5=tmp4.norm();
    auto tmp6 =(va-vb).norm();
}