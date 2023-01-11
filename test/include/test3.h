/*
 * @FilePath: test3.h
 * @Author: Taberwu
 * @Version: 2.0
 * @Date: 2022-12-06 19:42:52
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-12-06 22:16:54
 * Copyright: 2022 Taberwu. All Rights Reserved.
 * @Descripttion: 
 */
#ifndef _LEARN_TEST_3_H_
#define _LEARN_TEST_3_H_
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
// #include <opencv4/opencv2/core>
#include <eigen3/Eigen/Core>

namespace test{
    class Test3{
    public:
        Test3(){}
        Test3(std::string &filepath):filepath_(filepath)
        {
            std::string infile = filepath_ +"/input.txt";
            std::string outfile = filepath_ +"/output.txt";
            std::cout<<"filename "<<infile<<" "<<outfile<<std::endl;
            input_.open(infile.c_str(),std::ios::in);
            if(!input_.is_open()) std::cout<<"fail to open file"<<std::endl;
            output_.open(outfile.c_str(),std::ios::out|std::ios::trunc);
        }
        ~Test3() = default;
         void readtest(){
            std::string input_data;
            int i=1;
            
            std::uint64_t time =0;
            double num1,num2,num3;
            while(std::getline(input_,input_data)){
                Eigen::Vector3d lla = Eigen::Vector3d::Zero();
                std::stringstream data(input_data);
                data>>time>>num1>>num2>>num3;
                lla[0] = num1;
                lla[1] = num2;
                lla[2] = num3;
                std::cout<<time<<" "<<num1<<" "<<num2<<" "<<num3<<std::endl;
                auto tmp = Eigen::Vector3d::Ones();
                lla +=tmp;
                output_<<lla[0]<<" "<<lla[1]<<" "<<lla[2]<<std::endl;
                std::cout<<i<<" lines"<<std::endl;
                input_data.clear();
                data.str("");
                i++;
            }
            input_.close();
            output_.close();
        }

    private:
       
        std::string     filepath_;
        std::fstream    output_;
        std::fstream    input_;

    };
}
#endif