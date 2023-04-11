/**
****************************************************************************************
 * @FilePath: test12.hpp
 * @Author: Taber.wu
 * @Date: 2023-02-22 12:07:16
 * @LastEditors: 
 * @LastEditTime: 2023-02-22 12:07:16
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************
*/
#include <chrono>
#include <thread>
#include <mutex>
#include <string>
#include <iostream>
#include <condition_variable>
#include <deque>
#include <map>
#include <memory>
#include <functional>
#include <atomic>
#include <fstream>

struct Msg{
    std::uint64_t timestamp;
    int i;
    std::string source;

    Msg &operator =(const Msg other){
        this->timestamp = other.timestamp;
        this->i = other.i;
        this->source = other.source;
        return *this;
    }
};

class ThreadjoinTest{
public:
    ThreadjoinTest():is_run_(true)
    {
        index_ = 0;
    }

    ~ThreadjoinTest(){
        is_run_ = false;
        for(auto &thread :threads_){
            if(thread.joinable())
            {
                std::cout<<"join thread id "<<thread.get_id()<<std::endl;
                thread.join();
            }
                
        }
        
    }


    void init(){
        auto threadA = std::thread([&](){
            std::fstream file("threada.txt",std::ios::out|std::ios::trunc);
            auto id = std::this_thread::get_id();
            file<<"thread id "<<id<<std::endl;
            while(is_run_){
                auto testa = std::make_shared<Msg>();
                testa->timestamp = std::chrono::steady_clock::now().time_since_epoch().count();
                testa->source = "A";
                testa->i = index_;
                file<<testa->timestamp<<" "<<testa->i<<std::endl;
                {
                std::unique_lock<std::mutex>locker(locker_);
                datas_.emplace_back(testa);
                trigger_.notify_one();
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
            file.close();
        });
        threads_.emplace_back(std::move(threadA));

        threads_.emplace_back(std::thread([&](){
            std::fstream file("threadb.txt",std::ios::out|std::ios::trunc);
            auto id = std::this_thread::get_id();
            file<<"thread id "<<id<<std::endl;
            while(is_run_){{
                std::unique_lock<std::mutex>locker(locker_);
                datas_.emplace_back(std::make_shared<Msg>());
                datas_.back()->timestamp = std::chrono::steady_clock::now().time_since_epoch().count();
                datas_.back()->source = "B";
                datas_.back()->i = index_;
                file<< datas_.back()->timestamp<<" "<<datas_.back()->i<<std::endl;
                trigger_.notify_one();}
                std::this_thread::sleep_for(std::chrono::milliseconds(400));
            }
            file.close();
        }));

        auto threadc = std::thread([&](){
            std::fstream file("threadc.txt",std::ios::out|std::ios::trunc);
            auto id = std::this_thread::get_id();
            file<<"thread id "<<id<<std::endl;
            while(is_run_){
                Msg testc;
                testc.timestamp = std::chrono::steady_clock::now().time_since_epoch().count();
                testc.source = "c";
                testc.i = index_;
                file<<testc.timestamp<<" "<< testc.i<<std::endl;
                {
                   std::unique_lock<std::mutex>locker(locker_);
                   datas_.emplace_back(std::make_shared<Msg>(testc));
                   trigger_.notify_one();
                }
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            file.close();
        });
        threads_.emplace_back(std::move(threadc));
    }

    void run(){
        while(is_run_){
            index_++;
            {
            std::unique_lock<std::mutex>locker(locker_);
            while(datas_.empty()) trigger_.wait(locker);
            auto msg = datas_.front();
            std::cerr<<msg->timestamp<<" "<<msg->i<<" "<<msg->source<<std::endl;
            datas_.pop_front();
            }
            if(index_ > 100)
                is_run_ = false;
        }
    }

private:
    std::deque<std::shared_ptr<Msg>> datas_;
    bool is_run_;
    std::deque<std::thread>threads_;
    std::mutex locker_;
    std::condition_variable_any trigger_;
    std::atomic<int> index_;

};
