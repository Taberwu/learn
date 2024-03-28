/****************************************************************************************
 * @FilePath: vector_insert.hpp
 * @Author: Taber.wu
 * @Date: 2024-01-26 11:32:16
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-03-05 09:39:03
 * @Copyright: 2024 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef _INSERT_TEST_HPP_
#define _INSERT_TEST_HPP_
#include <vector>
#include <deque>
#include <iostream>
#include <memory>
#include <string>

 struct Frame{
    uint64_t stamp;
    uint8_t data[540*960];
    Frame(const int tim){
        stamp = tim;
        memset(data, 0, 540*960*sizeof(uint8_t));
    }
};
struct KeyFrame{
    uint64_t stamp;
    uint8_t kpt[100][2];
    KeyFrame(const int tim){
        stamp = tim;
        memset(kpt, 0, 100*2*sizeof(uint8_t));
    }
};

class InsertTest{
public:
    InsertTest(){}
    ~InsertTest(){}
    // void test(int j, int k){
    //     std::deque<std::shared_ptr<Frame>> que_data;
    //     for(int i = 1; i < k; i++ ){que_data.push_back(std::make_shared<Frame>(i*2+1));}
    //     std::vector<std::shared_ptr<Frame>> vector_data;
    //     vector_data.reserve(k);
    //     bool find_top = false, find_butomm = false;
    //     auto top = que_data.begin();
    //     auto buttom = que_data.rbegin();
    //     while((!find_top) || (!find_butomm)){
    //         if((*top)->stamp < j){top++;}
    //         else{find_top=true;}

    //         if((*buttom)->stamp > k){buttom++;}
    //         else{find_butomm=true;}

    //         if((*top)->stamp > (*buttom)->stamp) break;
    //     }
    //     if((*top)->stamp != j){
    //         vector_data.push_back(std::make_shared<Frame>(j));
    //     }
    //     std::vector<int> tmp ;
    //     auto trs = std::make_shared<Frame>(*(*top));
    //     while((*top)->stamp < (*buttom)->stamp){
    //         vector_data.push_back((*top));
    //         top++;
    //     }
    //     if((*buttom)->stamp != k){
    //         vector_data.push_back(std::make_shared<Frame>(k));
    //     }
    //     for(auto s:vector_data){
    //        std::cout<<s->stamp<<" ";
    //     }
    //     std::cout<<std::endl;
    // }

    // void test2(){
    //     std::deque<int> datas;
    //     for(int i = 0; i < 30; i++){
    //         datas.emplace_back(i*i);
    //     }
    //     auto data_ptr = datas.begin();
    //     while(data_ptr != datas.end()){
    //         auto sign = (*data_ptr == *datas.rbegin());
    //         data_ptr ++;
    //     }
    // }

    void test3(){
        std::vector<std::vector<double>> params(3, std::vector<double>(7, 0.));
        params[1][5] = 1.0;
        for(size_t k = 0; k<params.size(); k++){
            for(int i = 0; i<7; i++){
                printf(" %.3f ", params[k][i]);
            }
            printf("\n");
        }

    }
};

#endif