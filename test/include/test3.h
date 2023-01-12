/**
****************************************************************************************
 * @FilePath: test3.h
 * @Author: Taber.wu
 * @Date: 2022-12-12 08:56:53
 * @LastEditors: 
 * @LastEditTime: 2022-12-12 08:56:53
 * @Copyright: 2022 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************
*/
#ifndef _NOTES_TEST_3_H_
#define _NOTES_TEST_3_H_

#include <fstream>
#include <sstream>
#include <iomanip>
#include <istream>
#include <string>
#include <deque>

class TestThd{
public:
    TestThd(){}
    TestThd(std::string &filepath):filepath_(filepath)
    {
       
    }
    virtual ~TestThd() = default;
    void adjustTimestamp(std::uint64_t basetime, std::string infile, std::string outfile){
        std::string intarget,outtarget;
        intarget = filepath_+infile;
        outtarget = filepath_ + outfile;
        std::fstream indata(intarget.c_str(),std::ios::in);
        std::fstream outdata(outtarget.c_str(), std::ios::out|std::ios::trunc);
        std::string getstr,outstr;
        std::uint64_t timestamp; 
    
        while (std::getline(indata, getstr))
        {
            std::stringstream ss(getstr);
            outstr.clear();
            ss>>timestamp;
            outstr = std::to_string(timestamp - basetime);
            std::string tmp;
            while(ss>>tmp){
                outstr +=" ";
                outstr += tmp;
            }
            getstr.clear();
            ss.str("");
           
            outdata<<outstr<<std::endl;
            
        }
        indata.close();
        outdata.close();
    }
private:

    
    std::string filepath_;
    
};


#endif