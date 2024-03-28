/****************************************************************************************
 * @FilePath: bind_test.hpp
 * @Author: Taber.wu
 * @Date: 2024-03-27 14:46:46
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-03-28 10:00:29
 * @Copyright: 2024 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef BIND_TSET_HPP__
#define BIND_TSET_HPP__
#include <functional>
#include <vector>
#include <memory>
#include <string>
#include <iostream>

namespace bindtest{
    class BindTest{
        public:
        BindTest(){}
        ~BindTest(){}
        void test(){
            auto my_func = std::bind(&BindTest::bindadd, this,  std::placeholders::_1, (int)3);
            my_func(std::make_shared<int>(7));
        }
        void test2(const int data){
            for(int i = 4; i < 7; i++){
                auto my_func = std::bind(&BindTest::bind_mut_add, this,  std::placeholders::_1, i,  std::placeholders::_2);
                my_func(std::make_shared<int>(data), 9.f);
                auto ff = std::bind(&BindTest::bind_mut_add, this,  std::placeholders::_1, i,  std::placeholders::_2);
               f1_ = std::make_shared<std::function<void(const std::shared_ptr<int>, float)>>(ff);
               
            }   
        }
        protected:
         void bindadd(const std::shared_ptr<int> a, const int b){
            printf("a %d b %d\n", *a, b);

         }

         void bind_mut_add(const std::shared_ptr<int> a, const int b, float c){
            printf("a %d b %d %.2f\n", *a, b, c);

         }
         private:
         std::shared_ptr<std::function<void(const std::shared_ptr<int>, float)>> f1_;
         
    };


}

#endif