/**
****************************************************************************************
 * @FilePath: test4.h
 * @Author: Taber.wu
 * @Date: 2022-12-21 09:05:43
 * @LastEditors: 
 * @LastEditTime: 2022-12-21 09:05:43
 * @Copyright: 2022 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************
*/
#ifndef _NOTES_TEST_4_H_
#define _NOTES_TEST_4_H_
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared.hpp>

class TestF{
public:
    TestF(){
        
        test_ptr_.reset();
        if(test_ptr_ == nullptr){
            std::cout<<"make pullptr"<<std::endl;
            test_ptr_ = boost::make_shared<int>(1);
            std::cout<<" data "<<*test_ptr_.get()<<std::endl;
        }
        else 
            std::cout<<"test ptr is not null"<<std::endl;
    }
    ~TestF(){}

private:
    boost::shared_ptr<int> test_ptr_;
};



#endif