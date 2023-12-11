/****************************************************************************************
 * @FilePath: test18.hpp
 * @Author: Taber.wu
 * @Date: 2023-06-20 08:54:41
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-06-20 13:06:52
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef TEST_OPENV_18_HPP_
#define TEST_OPENV_18_HPP_

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <iostream>
#include <vector>

namespace EighteenTest{

class MemcpyMat{
public:
    MemcpyMat(){
        for(int i =0; i<5;i++){
            for(int j=0;j<5;j++){
                input_des[i][j] = i;
            }
        }
        output_des = cv::Mat(5, 5, CV_8U);
    }
public:
    void test(int i){
        uint8_t tmp = (uint8_t)i;
        for(int k =0; k<5;k++){
            for(int l=0;l<5;l++){
                input_des[k][l] = tmp;
                tmp++;
            }
            
        }
        for(int k = 0; k < 5; k++){
            auto ptr = output_des.ptr<uchar>(k);
            memcpy(ptr, &input_des[k][0], 5 * sizeof(uint8_t));
        }
        std::cout<<"input vector :"<<std::endl;
        for(int n=0; n<5; n++){
            for(int m=0; m<5;m++){
                std::cout<<(uint32_t)input_des[n][m]<<" ";
            }
            std::cout<<std::endl;
        }
        std::cout<<"-------------------------"<<std::endl;
        std::cout<<"out put mat: "<<std::endl;
        std::cout<<output_des<<std::endl;
        std::cout<<"-------------------------"<<std::endl;
        cv::Mat test2 = cv::Mat::ones(3,3,CV_32S) * -1;
        std::cout<<test2<<std::endl;
    }
private:
   uint8_t input_des[5][5];
   cv::Mat output_des;
};
}
#endif