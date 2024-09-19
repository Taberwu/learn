/*
 * @FilePath: test2.cpp
 * @Author: Taberwu
 * @Version: 2.0
 * @Date: 2024-08-27 20:43:07
 * @LastEditors: 
 * @LastEditTime: 2024-09-19 19:43:04
 * Copyright: 2024 Taberwu. All Rights Reserved.
 * @Descripttion: 
 */
#include <vector>
#include <memory>


class Pose{
    public:
    std::uint64_t stamp;
    double[3] t;
    double[4] q;
};

uint64_t period = 10;//ms


void getPos(const std::uint64_t stamp, const std::vector<Pose>& history, std::shared_ptr<Pose> &res){
  
  
    if((stamp  < history[0].stamp - period) ||(stamp  > history.back().stamp + period)){
        res = nullptr;
        return;
    }
    size_t i =0;
    for(; i < history.size(); i++){
        if(history[i].stamp >= stamp) break;
    }
    double k = 1.0;
    if((i ==0) ){
        k = 
    }
}