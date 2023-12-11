/****************************************************************************************
 * @FilePath: test23.hpp
 * @Author: Taber.wu
 * @Date: 2023-08-28 09:21:50
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-08-28 10:26:39
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef __STATI_STATE_TEST__
#define __STATI_STATE_TEST__
#include <iostream>
namespace normtest{

class Baseclass{
public:
    Baseclass(){}
    virtual ~Baseclass() = default;
    virtual void test() = 0;
    int a;
    static int static_a;
};

class SonclassA : public Baseclass{
public:
    SonclassA(const int &initlize){
        c = &initlize;
        a = 3;
    }
    ~SonclassA(){}
    void test(){
        printf("a: %d static_a : %d point c %d\n", this->a, this->static_a, *c);
    }
    void set_a(const int input){
        this->a = input;
    }

    const int *c;
};

class SonclassB : public Baseclass{
public:
    SonclassB(){}
    ~SonclassB(){}
    void test(){
        printf("b: %d static_a : %d\n", this->a, this->static_a);
        
    }
    void set_a(const int input){
        this->a = input;
    }
};


int normtest::Baseclass::static_a = 1;
}
#endif