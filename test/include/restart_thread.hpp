/****************************************************************************************
 * @FilePath: restart_thread.hpp
 * @Author: Taber.wu
 * @Date: 2024-02-29 15:13:05
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-02-29 16:57:52
 * @Copyright: 2024 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef _THREAD_RESATRT_HPP_
#define _THREAD_RESATRT_HPP_

#include <atomic>
#include <iostream>
#include <chrono>
#include <thread>
#include <memory>

namespace threadtest{
    class RestartThread{
        public:
            RestartThread():fd_(-1), value_(0), cnt_(0)
            {
                std::atomic_init(&reopen_, true);
                std::atomic_init(&checked_sts_, false);
                run_ptr_ = nullptr;
            }
            ~RestartThread(){
                uninit();
                close();
            }

            void test(){
                printf("start run ptr\n");
                run_ptr_ = std::shared_ptr<std::thread>(new std::thread(std::bind(&RestartThread::start, this)));
                printf("step while\n");
                while((cnt_ < 3)){
                    while(!checked_sts_){
                        int a =1;
                        std::this_thread::sleep_for(std::chrono::milliseconds(20));
                    }
                    if(!reopen_) {
                        printf("not need to reopen\n");
                        break;
                    }
                    checked_sts_.store(false);
                    cnt_++;
                    printf("reopen %d\n", cnt_);
                        if(run_ptr_->joinable()){
                            run_ptr_->join();
                        }
                        run_ptr_.reset();
                        uninit();
                        close();
                        run_ptr_ = std::shared_ptr<std::thread>(new std::thread(std::bind(&RestartThread::start, this)));
                }
                run_ptr_->join();
                printf("finish ++++\n");
            }
        protected:
            bool open(){
                if(fd_ < 0){
                    fd_ = 758 + cnt_;
                }
                else{
                    fd_ += 2;
                }
                if((fd_ % 2)) return true;
                return false;
            }
            void close(){
                fd_ = -1;

            }
            bool init(){
                value_ = std::chrono::system_clock::now().time_since_epoch().count();
                auto sts = (value_ % 99);
                if(sts > 45) {
                    std::cerr<<"fail to init with sts "<< sts<<std::endl;;
                    return false;
                }
                return true;
            }
            void uninit(){
                value_ = 0;
            }
            void start(){
                printf("#####start thread#######\n");
                if(!open()){
                    printf("fail to open %d \n", fd_);
                    checked_sts_.store(true);
                    reopen_.store(true);
                    return;
                }
                    
                int init_cnt = 0;
                while(!init()){
                    std::this_thread::sleep_for(std::chrono::milliseconds(20));
                    if(init_cnt > 10) {
                        std::cerr<<"init over 10 times"<<std::endl;
                        checked_sts_.store(true);
                        reopen_.store(true);
                        return;
                    }
                    init_cnt ++;
                   std::cerr<<"try to init "<< init_cnt<<std::endl;
                }
                int run_cnt = 0;
                printf("fd %d value %lu\n", fd_, value_);
                checked_sts_.store(true);
                reopen_.store(false);
                while(run_cnt < 3){
                   
                    printf("------------------------\n");
                    run_cnt++;
                    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
                }
            }
        private:
            int fd_;
            std::uint64_t value_;
            std::atomic_bool reopen_;
            std::atomic_bool checked_sts_;
            std::shared_ptr<std::thread> run_ptr_;
            int cnt_;
    };
}
#endif