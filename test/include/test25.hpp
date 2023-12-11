/****************************************************************************************
 * @FilePath: test25.hpp
 * @Author: Taber.wu
 * @Date: 2023-09-20 14:53:10
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-09-20 16:32:20
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef _DQUENE_END_TEST_HPP_
#define _DQUENE_END_TEST_HPP_
#include <deque>
#include <memory>
#include <string>
#include <iostream>
#include <unistd.h>

#include <Eigen/Core>
#include <Eigen/Geometry>

namespace quetest{
    struct State {
        std::uint64_t timestamp;
        Eigen::Vector3d trans;
        Eigen::Quaterniond rot;
    };

    class EndTest{
        public:
        EndTest(){
            std::uint64_t stamp = 15900000000;
            int que_size = (int)(Eigen::VectorXd::Random(1)(0) * 50);
            int que_ptr_size = (int)(Eigen::VectorXd::Random(1)(0) * 50);
            que_size = (que_size > 3)?que_size:3;
            que_ptr_size = (que_ptr_size > 3)?que_ptr_size:3;
            for(int i = 0; i < que_size; i++){
                auto sts = std::make_shared<State>();
                sts->timestamp = stamp + (std::uint64_t)(1e8 * i);
                sts->trans = Eigen::Vector3d(1.0, 2.0, 0.005) * i;
                sts->rot = Eigen::Quaterniond::FromTwoVectors(Eigen::Vector3d(0.,0.,1.), (Eigen::Vector3d(0.,0.,1.) + i * Eigen::Vector3d(0.01745,0.,0.)));
                state_que_.push_back(sts);
            }
            state_que_ptr_ = std::make_shared<std::deque<std::shared_ptr<State>>>();
            for(int i = 0; i < que_size; i++){
                auto sts = std::make_shared<State>();
                sts->timestamp = stamp + (std::uint64_t)(0.8 * 1e8 * i);
                sts->trans = Eigen::Vector3d(1.0, 2.0, 0.005) * i;
                sts->rot = Eigen::Quaterniond::FromTwoVectors(Eigen::Vector3d(0.,0.,1.), (Eigen::Vector3d(0.,0.,1.) + i * Eigen::Vector3d(0.01745,0.,0.)));
                state_que_ptr_->push_back(sts);
            }

        }
        virtual ~EndTest() = default;

        void test(){

            printf("que size %lu  back stamp %lu [%.7f %.7f %.7f]\n", state_que_.size(), state_que_.back()->timestamp, 
                state_que_.back()->trans.x(), state_que_.back()->trans.y(), state_que_.back()->trans.z());
            printf("queptr size %lu  back stamp %lu [%.7f %.7f %.7f]\n", state_que_ptr_->size(), state_que_ptr_->back()->timestamp,
                state_que_ptr_->back()->trans.x(), state_que_ptr_->back()->trans.y(), state_que_ptr_->back()->trans.z());
                printf("==============================\n");
            auto ptr_iter = state_que_ptr_->begin();
            for(; ptr_iter!= state_que_ptr_->end(); ptr_iter++){
                int i = (ptr_iter== state_que_ptr_->end());
                printf("queptr size %lu  back stamp %lu [%.7f %.7f %.7f]    %d\n", state_que_ptr_->size(), ptr_iter->get()->timestamp,
                ptr_iter->get()->trans.x(), ptr_iter->get()->trans.y(), ptr_iter->get()->trans.z(), i);
            }
            printf("--------------------------\n");

            auto iter2 = state_que_ptr_->begin();
            while(std::next(iter2) != state_que_ptr_->end()) iter2++;
            int key = (iter2== state_que_ptr_->end());
            printf("iter2 reach que end %d\n", key);
            printf("queptr size %lu  back stamp %lu [%.7f %.7f %.7f]\n", state_que_ptr_->size(), iter2->get()->timestamp,
                iter2->get()->trans.x(), iter2->get()->trans.y(), iter2->get()->trans.z());
            printf("**************************\n");
            ++iter2;
            int key2 = (iter2== state_que_ptr_->end());
            printf("iter reach que end %d\n", key2);
            
        }
        protected:

        private:
            std::deque<std::shared_ptr<State>> state_que_;
            std::shared_ptr<std::deque<std::shared_ptr<State>>> state_que_ptr_;
    };

}

#endif