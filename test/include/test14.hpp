/****************************************************************************************
 * @FilePath: test14.hpp
 * @Author: Taber.wu
 * @Date: 2023-03-02 08:59:36
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-03-03 09:02:09
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef _TEST_BFILEWRITE_TEST_
#define _TEST_BFILEWRITE_TEST_
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>


class BinaryFileWrite{
public: 
    BinaryFileWrite(){};
    BinaryFileWrite(const std::string filepath):filepath_(filepath){
        chwrite_.open(filepath_+"/ch.txt", std::ios::binary|std::ios::trunc|std::ios::out);
        lenwrite_.open(filepath_+"/len.txt", std::ios::binary|std::ios::trunc|std::ios::out);
    }
    void lenWrite(const unsigned char* buf, size_t len){
        lenwrite_.write(reinterpret_cast<const char*>(buf), len);
    }
    void chwrite(const unsigned char* buf, size_t len){
        for(int i=0; i<(int)len;i++){
            chwrite_<<std::hex<<std::setw(2)<<std::setfill('0')<<(int)buf[i]<<" ";
        }
        chwrite_<<std::defaultfloat<<std::endl;
    }


private:
    std::string filepath_;
    std::fstream chwrite_;
    std::fstream lenwrite_;
};




#endif