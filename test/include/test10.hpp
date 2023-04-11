/**
****************************************************************************************
 * @FilePath: test10.hpp
 * @Author: Taber.wu
 * @Date: 2023-02-14 10:17:37
 * @LastEditors: 
 * @LastEditTime: 2023-02-14 10:21:18
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************
*/


#ifndef _TEST_TEN_HPP_
#define _TEST_TEN_HPP_

#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <mutex>
#include <cmath>
#include <unistd.h>
#include <condition_variable>
#include <string>

namespace test_thread{
    template <typename T>
    class Hate{
    public:
        Hate(){}
        ~Hate() = default;

        Hate(const std::string& name,const  std::function<void (T&)> &handler, const std::uint64_t period_=200 ):
            handler_(handler), name_(name), period_(period_){
                init();
                std::thread operate(&Hate::run, this);
                operate.detach();
            }

        void init(){
            data_ = T(M_PI);
        }

        void run(){
            int i=1;
            while(true){
                {
                std::unique_lock<std::mutex>locker(lock_);
                handler_(data_);
                std::cout<<name_<<"  "<<data_<<std::endl;
                con_.notify_one();
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(period_));
            }
        }

        const T getdata(){

             std::unique_lock<std::mutex>locker(lock_);
             con_.wait(locker);
             
             return data_;
        }
  
        
    private:
       T data_;
       std::function< void (T&)> handler_;
       std::uint64_t period_;
       std::string name_;
       std::mutex lock_;
       std::condition_variable con_;
    };
}

#endif