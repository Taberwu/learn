/****************************************************************************************
 * @FilePath: test27.hpp
 * @Author: Taber.wu
 * @Date: 2023-10-18 14:08:39
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-10-19 10:12:50
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef TEST27__HPP_
#define TEST27__HPP_
#include <deque>
#include <map>
#include <memory>
#include <string>
#include <Eigen/Core>
#include <Eigen/Geometry>

struct GloabalData{
    std::uint64_t timestamp;
    Eigen::Vector3d trans;
    Eigen::Quaterniond rot;
};

struct PPData{
    int id;
    float rgb[3] ={0.f};
    std::deque<std::shared_ptr<GloabalData>> ppdatas;
};

class JMT{
public:
    JMT(){}
    ~JMT() = default;
 void test(){
     PPData tmp_data;
     tmp_data.id = 1;
     tmp_data.ppdatas.clear();
     data_cates["a"] = tmp_data;
     tmp_data.id = 2;
     data_cates["b"] = tmp_data;
     tmp_data.id = 3;
     data_cates["c"] = tmp_data;
     tmp_data.id = 4;
     data_cates["a"] = tmp_data;
     tmp_data.id = 5;
     data_cates["e"] = tmp_data;
    Eigen::Quaterniond::Identity();
    for(auto &ppdta :data_cates){
        printf("%s  %d  %.3f\n",ppdta.first.c_str(), ppdta.second.id, ppdta.second.rgb[1]);
    }
 }

private:
    std::map<std::string, PPData> data_cates;
};

#endif