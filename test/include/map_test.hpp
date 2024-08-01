/****************************************************************************************
 * @FilePath: map_test.hpp
 * @Author: Taber.wu
 * @Date: 2024-07-01 18:00:28
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-07-01 18:21:48
 * @Copyright: 2024 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef __DRE_MAP_TSET_HPP__
#define __DRE_MAP_TSET_HPP__
#include <map>
#include <memory>
#include <iostream>
#include <chrono>
#include <thread>

namespace maptest{
struct Mdata
{
    Mdata(const int index){
        stamp = std::chrono::system_clock::now().time_since_epoch().count();
        idx = index;
    }
    std::uint64_t stamp;
    int idx;
};


class MapTest{
public:
    MapTest(){}
    ~MapTest()=default;
    void test(){
        std::map<int64_t, std::pair<std::shared_ptr<Mdata>, int>> map_data;
        for(int i = 0; i < 9; i++){
            auto tmp_data = std::make_shared<Mdata>(i);
            int64_t id = (int64_t)(tmp_data.get());
            int k = i%2;
            map_data.insert(std::make_pair(id, std::make_pair(tmp_data, k)));
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        for(auto data : map_data){
            printf("id %ld  stamp %lu index %d  k %d\n", data.first, data.second.first->stamp,  data.second.first->idx, data.second.second);
        }
        printf("====================================\n");
        int cnt = 2;
        std::vector<int64_t> ids;
        for(auto &data : map_data){
            if(data.second.second %2 == 0){
               ids.push_back(data.first);
            }
            else{
                data.second.second += cnt;
                cnt += 2;
            }
        }
        for(auto id : ids){
            map_data.erase(id);
        }
        for(auto data : map_data){
            printf("id %ld  stamp %lu index %d  k %d\n", data.first, data.second.first->stamp,  data.second.first->idx, data.second.second);
        }
        

    }
};
}



#endif