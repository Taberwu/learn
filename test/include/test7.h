/**
****************************************************************************************
 * @FilePath: test7.h
 * @Author: Taber.wu
 * @Date: 2023-02-02 09:29:46
 * @LastEditors: 
 * @LastEditTime: 2023-02-02 09:29:46
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************
*/
#ifndef _NOTES_TEST_7_H_
#define _NOTES_TEST_7_H_

#include <ctime>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <shared_mutex>
#include <mutex>
#include <string>
#include <map>
#include <memory>
#include <iostream>
#include <thread>


namespace Test{

struct Recorder{
    FILE *file;
    std::recursive_mutex locker;
};

struct DataA{
    double a1;
    float  a2;
    uint32_t a3;
    std::string a4; 
    std::int8_t sts;
};
struct DataB{
    std::string b1;
    std::uint8_t sts;
    float b2;
    int b3;
    
};

class TestSev{
public:
    TestSev(){
        i_ = 0;
    }
    ~TestSev(){
        for(auto &record : recorders_){
            fclose(record.second->file);
        }
    }
    bool register_recorder(std::string name, std::string path = {"/home/joyson/Desktop/tmp/sinout_test/"}){
        if(recorders_.find(name) == recorders_.end()){
            std::string fullname = path + name+"_";
            std::time_t now = std::time(0);
            std::tm *gmtm = std::gmtime(&now);
            fullname += (std::to_string(gmtm->tm_year +1900) +"_"+std::to_string(gmtm->tm_mon+1)+"_"+ std::to_string(gmtm->tm_mday)+"-");
            fullname += (std::to_string(gmtm->tm_hour)+":"+std::to_string(gmtm->tm_min)+":"+std::to_string(gmtm->tm_sec));
            fullname += ".txt";
            auto rec = std::make_shared<Recorder>();
            rec->file = fopen(fullname.c_str(), "w");
            recorders_.insert({name, rec});
            return true;
        }
        std::cerr<<"recorder is exist "<<std::endl;
        return false;
    }

    

    template<typename T ,typename ...Args >
    bool record(const std::string name, const std::string &data_format, const T &value,  Args... args){
        
        {
        std::lock_guard<std::recursive_mutex> lock(recorders_[name]->locker);
        std::string format = data_format;
        std::cout<<"sizeof args "<<sizeof...(args)<<std::endl;
        myfprintf(name, format, value);
        record(name, format, args...);
        return true;
        }
    }
    
private:
    template< typename T>
    bool myfprintf(const std::string name, std::string &data_format, const T& value){
        if(recorders_.find(name) == recorders_.end()){
            std::cerr<<" recorder not exist"<<std::endl;
            return false;
        }

        std::string sub_str = data_format;
        int index = sub_str.find_first_of('%');
        int length = sub_str.size();
        if( index >-1){
          index = sub_str.find_first_of('%');
          length = sub_str.size();
          std::string format = sub_str.substr(0, index+1);
          sub_str = sub_str.substr(index+1, length-index);
          index = sub_str.find_first_of('%');
          if(index == 0){
            std::cerr<<"wrong format string"<<std::endl;
            return false;
          }
          format += sub_str.substr(0, index);
          int first = (index > 0)?index-1:0;
          length = sub_str.size();
          sub_str = sub_str.substr(first+1, length-first);
         fprintf(recorders_[name]->file, format.c_str(), value);
         std::cout<<i_<<" #"<<format<<" "<< value<<"     #"<<sub_str<<std::endl;
         i_++;
         data_format = sub_str;
         
         return true;
        }
        return false;
        
    }

    bool record(const std::string name, std::string &data_format){
            if(recorders_.find(name) == recorders_.end()){
                std::cerr<<" recorder not exist"<<std::endl;
                return false;
            }
            std::cout<<"record null parmater"<<std::endl;
            return true;
    }
    
    std::map<std::string, std::shared_ptr<Recorder>> recorders_;
    int i_;

};


}
#endif