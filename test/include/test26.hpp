/****************************************************************************************
 * @FilePath: test26.hpp
 * @Author: Taber.wu
 * @Date: 2023-09-27 16:10:43
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-09-27 16:36:21
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef __DEQUE_TEST_HPP_
#define __DEQUE_TEST_HPP_

#include <iostream>
#include <memory>
#include <deque>
#include <unistd.h>

#include <random>
#include <Eigen/Core>
#include <Eigen/Geometry>

struct Pose3{
    std::uint64_t timestamp;
    Eigen::Vector3d trans;
    Eigen::Quaterniond rotation;
};


class DequeTest{
public:
    DequeTest(){}
    ~DequeTest() = default;
    void test(){
        std::deque<std::shared_ptr<Pose3>> test_pos;
        for(int i=0; i<25; i++){
            auto pos = std::make_shared<Pose3>();
            pos->timestamp = i*10000000;
            pos->trans = Eigen::Vector3d(1., -2.5, 0.) *i;
            pos->rotation = Eigen::Quaterniond::UnitRandom();
            test_pos.emplace_back(pos);
        }
        int index = 25 -20;
        poses_.assign(test_pos.begin()+index, test_pos.end());
        printf("testpos size %lu, posre_ size %lu\n",test_pos.size(), poses_.size());
        printf("[%d]  %lu [%.5f %.5f %.5f]:testpos\n",5, test_pos[5]->timestamp, test_pos[5]->trans.x(), test_pos[5]->trans.y(), test_pos[5]->trans.z());
        printf("[%d]  %lu [%.5f %.5f %.5f]:testpos\n",24, test_pos[24]->timestamp, test_pos[24]->trans.x(), test_pos[24]->trans.y(), test_pos[24]->trans.z());
        printf("[%d]  %lu [%.5f %.5f %.5f]:poses_\n",0, poses_[0]->timestamp, poses_[0]->trans.x(), poses_[0]->trans.y(), poses_[0]->trans.z());
        printf("[%d]  %lu [%.5f %.5f %.5f]:poses_\n",19, poses_[19]->timestamp, poses_[19]->trans.x(), poses_[19]->trans.y(), poses_[19]->trans.z());
    }

private:
    std::deque<std::shared_ptr<Pose3>> poses_;

};


#endif