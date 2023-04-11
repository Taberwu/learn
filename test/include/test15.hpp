/****************************************************************************************
 * @FilePath: test15.hpp
 * @Author: Taber.wu
 * @Date: 2023-03-07 10:07:10
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-03-08 11:09:17
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <unistd.h>

class ReadHexfile{
public:
    ReadHexfile(std::string name):filename_(name)
    {
        file_.open(filename_.c_str(), std::ios::in);
        std::string outname = filename_.substr(0, filename_.length()-4)+"_origin.txt";
        output_file_.open(outname.c_str(), std::ios::out|std::ios::trunc|std::ios::binary);
    }
    ~ReadHexfile() = default;
    
    void getbuf(){
        if(!file_.is_open())
            return;
        std::string read_str ="";
        std::stringstream ss;
        while(std::getline(file_, read_str)){
            ss.str(read_str);
            auto *buf  = new std::uint16_t[255];
            int length = 0; 
            while(ss>>std::hex>>buf[length]){
                length++;
            }
            ss.clear();
            auto *output = new char[255];
            for(int k = 0; k<length; k++){
                output[k] = static_cast<char>(buf[k]);
            }
            //publish buf and length
            /*
            
            */
           std::cout<<std::dec<<"length "<<length<<std::endl;
           for(int n = 0; n<length;n++){
            std::cout<<std::hex<<(uint) output[n]<<" ";
           }
           std::cout<<std::dec<<std::endl;
            
            output_file_.write(output, length);
            delete []buf;
            delete []output;
            usleep(10);
        }
    }

    
private:
    std::string filename_;
    std::fstream file_;
    std::fstream output_file_;
};