/**
****************************************************************************************
 * @FilePath: test11.h
 * @Author: Taber.wu
 * @Date: 2023-02-17 17:34:57
 * @LastEditors: 
 * @LastEditTime: 2023-02-17 17:34:57
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************
*/
#ifndef _TEST_11_H_
#define _TEST_11_H_

#include <functional>
#include <thread>
#include <mutex>

#include "test8.h"

void f11(TestEi::DataA &data){
    std::cout<<"++++++run outer class fun"<<std::endl;
    data.a_copy +=1;
}

class BrainToolBox
{
public:
    void Add(TestEi::DataA &a) { 
        a.a_copy +=7;
        }
};


class Funcpt{
public:
    Funcpt(int a)
    {
        inter_data_ = std::make_shared<TestEi::DataA>(TestEi::Database(a));
        callpt_.callback = nullptr;
    }
    Funcpt(Funcpt* other){
        this->inter_data_ = other->inter_data_;
        this->callpt_ = other->callpt_;
    }

    void testcall( void (*callback)( TestEi::DataA&) ,int a){
        TestEi::DataA* b = new TestEi::DataA (TestEi::Database(a));
        
        callback(*b);
        std::cout<<"tail data "<<b->a_copy<<std::endl;
        delete b;
    }


    std::function<void( TestEi::DataA&)> call_func(){
        auto t = [&]( TestEi::DataA& data){
                inter_data_->a_copy +=3;
        };
        return t;
    }

    static void inf_t ( TestEi::DataA& data){
        std::cout<<"######run static class func"<<std::endl;
        data.a_copy +=5;
    }

    void inp_t ( TestEi::DataA& data){
        // std::cout<<" normal input "<<inter_data_->a_copy<<std::endl;
        std::cout<<"---------run normal class func"<<std::endl;
        this->inter_data_->a_copy +=7;
        data.a_copy = inter_data_->a_copy;
        std::cout<<" normal copy "<<data.a_copy<<std::endl;
    }
    void testcall2(){
        callpt_.clsscall = &Funcpt::inp_t;
        testcall(callpt_.callback, 2);
        
        
    }
    
    std::shared_ptr< TestEi::DataA> inter_data_;

    private:
        union callpt{
            void (*callback)( TestEi::DataA&);
            void (Funcpt::*clsscall)(TestEi::DataA&);
            void (Funcpt::*csscall(Funcpt * ))(TestEi::DataA&);
        }callpt_;

       
};



#endif