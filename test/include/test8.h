/**
****************************************************************************************
 * @FilePath: test8.h
 * @Author: Taber.wu
 * @Date: 2023-02-03 11:25:07
 * @LastEditors: 
 * @LastEditTime: 2023-02-03 11:25:07
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************
*/
#ifndef _TEST_EIGHT_H_
#define _TEST_EIGHT_H_

#include <any>
#include <cmath>
#include <chrono>
#include <iostream>
#include <functional>
#include <iostream>
#include <string>
#include <memory>
#include <deque>
#include <map>
#include <cxxabi.h>
#include <typeinfo>
#include <typeindex>

template<typename T>
struct TypeParseTraits{
    static const char* name;
};
#define REGISTER_PARSE_TYPE(X) \
    template<> const char* TypeParseTraits<X>::name = #X



namespace TestEi{

class Database{
public:
    Database(){
        a = M_PI;
    }
    Database(double input){
        a = input;
    }
    void run(){}
    Database& operator= (const Database &other){
        this->a = other.a;
        return *this;
    }
    virtual ~Database() = default;
    double a;
};

class DataA :Database
{
public:
    DataA(){   
    }
    DataA(Database database){
        this->a = database.a;
        this->a_copy = (int)(this->a);
    }
    ~DataA(){
        std::cout<<a_copy<<std::endl;
        std::cout<<"delete DataA"<<std::endl;
    }
    void run(){
       
    }
    int a_copy;
};

class DataB :Database{
public:
    DataB(){  
    }
    DataB(Database databse){
        this->a = databse.a;
        this->a_copy = std::to_string(this->a);
    }
    ~DataB(){
        std::cout<<a_copy<<std::endl;
        std::cout<<"delete DataB"<<std::endl;
    }
    void run(){
        
    }
    std::string a_copy;
};

class Sub{
    public: 
        Sub(){}
        ~Sub() = default;
        void run(){
            // for(auto listener: listeners_){
            //     std::cout<<"run "<<std::endl;
            //     Database a(1.0);
            //     listener->f(a);
            // }
        }
 
        class Listener{
            public:
                Listener(){}
                ~Listener()=default;
            private:
                    double a;
                    std::string str;
            };
      
};
}

typedef std::shared_ptr<TestEi::Database> DatabasePtr;

// template < typename T>
// const std::string getTypeName(){
//     std::string str = abi::__cxa_demangle(typeid(T).name(),NULL, NULL, NULL);
//     std::cout<<str<<"####"<<std::endl;
//     return str;
// }



#endif