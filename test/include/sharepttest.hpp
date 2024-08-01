/****************************************************************************************
 * @FilePath: sharepttest.hpp
 * @Author: Taber.wu
 * @Date: 2024-07-03 16:17:45
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-07-04 10:58:44
 * @Copyright: 2024 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef __SHAREDPTR_TEST_HPP___
#define __SHAREDPTR_TEST_HPP___

#include <memory>
#include <deque>
#include <iostream>
#include <string>

namespace dequetest{
    struct DataA{
        uint64_t stamp;
        int a;
        DataA():stamp(0), a(1)
        {}
        DataA(std::uint64_t t){
            stamp = t;
            a = (t/10)%10;
        }
    };
    struct DataB{
        char a;
        uint64_t stamp;
        DataB():stamp(0), a('a')
        {}
        DataB(std::uint64_t t){
            stamp = t;
            a = 'a' + ((t/5)%26);
        }
    };

    struct Res {
    public:
        std::deque<std::shared_ptr<DataA>> As;
        std::deque<std::shared_ptr<DataB>> Bs;
        Res(){
            As.clear();
            Bs.clear();
        }

        Res(const std::deque<std::shared_ptr<DataA>> as, std::deque<std::shared_ptr<DataB>> bs)
        :As(as), Bs(bs)
        {}
        void insert(const std::shared_ptr<DataA> &a, const std::shared_ptr<DataB> &b){
            As.push_back(a);
            Bs.push_back(b);
        }
        void clear(){
            As.clear();
            Bs.clear();
        }
        void show(){
            if(As.size() != Bs.size()){printf("sync fail\n");}
            for(size_t k = 0; k < As.size(); k++){
                printf("A stamp %lu mempath %lu, B stamp %lu char %c\n", As[k]->stamp, &(As[k]), Bs[k]->stamp, Bs[k]->a);
            }
        }
    };
    class DeqPtr{
    public:
        DeqPtr(){}
        ~DeqPtr() = default;
        void test(){
            for(uint64_t pos = 100; pos < 190; pos+=20){
                dataAs_.push_back(std::make_shared<DataA>(pos));
                dataBs_.push_back(std::make_shared<DataB>(pos+10));
            }
            auto res = std::make_shared<Res>();
            int a_cnt = 0, b_cnt = 0;
            sync(a_cnt, b_cnt, 0, 138, res);
            res->show(); 
            printf("k1  imu 0 ptr use cnt %ld\n",res->As[0].use_count());
            last_res_ = std::make_shared<Res>(*res);
            printf("------------------- sync a %d sync b %d\n", a_cnt, b_cnt);
            printf("k2  imu 0 ptr use cnt %ld\n",res->As[0].use_count());
            res->clear();
            printf("k3 imu 0 ptr use cnt %ld\n", dataAs_[0].use_count()); 
            dataAs_.erase(dataAs_.begin(), std::next(dataAs_.begin(), a_cnt));
            dataBs_.erase(dataBs_.begin(), std::next(dataBs_.begin(), b_cnt));
            std::deque<std::shared_ptr<DataA>> add_A;
            std::deque<std::shared_ptr<DataB>> add_B;
            for(uint64_t pos = 200; pos < 300; pos+=20){
                add_A.push_back(std::make_shared<DataA>(pos));
                add_B.push_back(std::make_shared<DataB>(pos+10));
            }
            dataAs_.insert(dataAs_.end(), add_A.begin(), add_A.end()); add_A.clear();
            dataBs_.insert(dataBs_.end(), add_B.begin(), add_B.end()); add_B.clear();
            sync(a_cnt, b_cnt, 138, 275, res);
            res->show(); 
            printf("-------------- sync a %d sync b %d\n", a_cnt, b_cnt);
            printf("k5 imu 0 ptr use cnt %ld\n",res->As[0].use_count());
        }


        void test2(){
            std::deque<std::shared_ptr<DataB>> dque1, dque2;
            std::deque<std::shared_ptr<DataA>> que1;
            auto last_res = std::make_shared<Res>();
            {
                dque1.push_back(std::make_shared<DataB>(300));
                printf("K1 datab use cnt %ld char %c \n", dque1[0].use_count(), dque1[0]->a);
                dque2.insert(dque2.end(), dque1.begin(), dque1.end()); 
                printf("K2 datab use cnt %ld char %c\n", dque2[0].use_count(), dque2[0]->a);
                dque1.clear();
                printf("K3 datab use cnt %ld char %c\n", dque2[0].use_count(), dque2[0]->a);
                auto res1 = std::make_shared<Res>(que1, dque2);
                printf("K4 datab use cnt %ld char %c\n", res1->Bs[0].use_count(), res1->Bs[0]->a);
                dque2.pop_front();
                printf("K5 datab use cnt %ld char %c\n", res1->Bs[0].use_count(), res1->Bs[0]->a);
                last_res->Bs = res1->Bs;
                printf("K6 datab use cnt %ld char %c\n", last_res->Bs[0].use_count(), last_res->Bs[0]->a);
            }
            printf("------------------\n");
            printf("K7 datab use cnt %ld char %c\n", last_res->Bs[0].use_count(), last_res->Bs[0]->a);
           
        }

    protected:
        void sync(int &inertial_count, int &wheel_count, uint64_t s, uint64_t e, std::shared_ptr<Res> &result){
            if(last_res_) {
                result = last_res_;
                 printf("get last data data size %lu \n", result->As.size());
            }
            while (!dataAs_.empty()) {
                auto& inertial = dataAs_.front();
                if (inertial->stamp >= s) {
                    break;
                }
                dataAs_.pop_front();
            }
        while (!dataBs_.empty()) {
            auto& wheel = dataBs_.front();
            if (wheel->stamp >= s) {
                break;
            }
            dataBs_.pop_front();
        }

    inertial_count = 0, wheel_count = 0;
    while(true){
        int inertial_last = std::max<int>(0, inertial_count - 1);
        int wheel_last = std::max<int>(0, wheel_count - 1);
        auto inertial = dataAs_[inertial_count];
        auto wheel = dataBs_[wheel_count];
        std::uint64_t inertial_stamp = inertial->stamp;
        std::uint64_t wheel_stamp = wheel->stamp;
        if(inertial_stamp >= e && wheel_stamp >= e){
            auto inertial_synced_end_ = std::make_shared<DataA>(*inertial);
            inertial_synced_end_->stamp =  e;
            auto wheel_synced_end_ = std::make_shared<DataB>(*wheel);
            wheel_synced_end_->stamp = e;
            result->insert(inertial_synced_end_, wheel_synced_end_);
            printf("insert imu %lu wheel %lu\n", inertial_synced_end_->stamp, wheel_synced_end_->stamp);
            break;
        }
        if(inertial_stamp == wheel_stamp){
            result->insert(inertial, wheel);
            printf("insert imu %lu wheel %lu char %c\n", inertial->stamp, wheel->stamp, wheel->a);
            inertial_count ++; wheel_count ++;
        }else if(inertial_stamp > wheel_stamp){
            auto ip = std::make_shared<DataA>(*dataAs_[inertial_last]);
            ip->stamp = (wheel_stamp);
            result->insert(ip, wheel);
            printf("insert imu %lu wheel %lu char %c\n", ip->stamp, wheel->stamp, wheel->a);
            wheel_count ++;
        }else{
            auto wp = std::make_shared<DataB>(*dataBs_[wheel_last]);
            wp->stamp = (inertial_stamp);
            result->insert(inertial, wp);
            printf("insert imu %lu wheel %lu char %c, imu shared_ptr cnt %ld\n", inertial->stamp, wp->stamp, wp->a, inertial.use_count());
            inertial_count ++;
        }
    }

        }

    private:
        std::deque<std::shared_ptr<DataA>> dataAs_;
        std::deque<std::shared_ptr<DataB>> dataBs_;
        std::shared_ptr<Res> last_res_;
    };
}


#endif