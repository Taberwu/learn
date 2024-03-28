/****************************************************************************************
 * @FilePath: typetest.hpp
 * @Author: Taber.wu
 * @Date: 2024-03-28 10:00:42
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-03-28 18:12:26
 * @Copyright: 2024 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef _TSET_TYPS_ID_HPP___
#define _TSET_TYPS_ID_HPP___
#include <type_traits>
#include <typeinfo>
#include <shared_mutex>
#include <memory>
#include <map>
#include <functional>
#include <deque>
#include <tuple>
#include <any>
#include <utility>
#include <deque>
#include <vector>

namespace tamplate_func{

struct Imu
{
public:
    std::uint64_t stamp;
    Imu(std::uint64_t t){
        stamp = t;
    }
};

struct Image
{
public:
    struct Header
    {
        std::uint64_t stamp;
    } header;
    char data;
    Image(std::uint64_t t){
        header.stamp = t;
        data = 'c';
    }
};
struct Wheel
{
public:
   std::pair<int , std::uint64_t> pair_head; 
    Wheel(std::uint64_t t){
        pair_head.first = 0;
        pair_head.second = t;
    }
    
};



template <typename... MsgsType>
class MutableSynchronizer {
public:
    using StampFuncTypeMap = std::map<std::string, std::shared_ptr<std::any>>;
    using DataDequeType = std::vector<std::deque<std::pair<std::uint64_t, std::any>>>;
    MutableSynchronizer(){
        sync_num_ = sizeof...(MsgsType);
        
        ds_.resize(sync_num_);
    }
    virtual ~MutableSynchronizer() = default;
    template <typename MsgGetTime, typename ...OtherMsgGetTime>
    void setMsgTimestampFunction(const MsgGetTime& get_time, const OtherMsgGetTime&... other_gettime){
        inter_setMsgTimestampFunction(get_time);
        registerGetMsgTimestampFunction(other_gettime...);
    }
    template <typename MsgGetTime>
    void setMsgTimestampFunction(const MsgGetTime& get_time){
        inter_setMsgTimestampFunction(get_time);
    }

    template<int pos, typename MsgType>
    bool insert(std::shared_ptr<MsgType> &msg){
        std::string type_name = typeid(MsgType).name();
        if(timestamp_func_map_.find(type_name) == timestamp_func_map_.end()){
            printf("not set %s getTimestamp function \n", type_name.c_str());
            return false;
        }
        std::uint64_t stamp = timestamp_func_map_[type_name](msg);
        if((pos > msg_type_.size() -1) || (type_name != msg_type_[pos])){
            printf("pos %d type not fix\n", pos);
            return false;
        }
        std::unique_lock locker(locker_);
        ds_[pos].emplace_back(std::make_pair(stamp, msg));
        return true;
    }
    void registerHandler(std::function<void(const MsgsType& ...)> &callback){
        handler_ = callback;
    }

protected:
   

    template <typename MsgType>
    void inter_setMsgTimestampFunction(std::function<std::uint64_t(const std::shared_ptr<MsgType>)> &get_timestamp){
        std::string type_name = typeid(MsgType).name();
        auto func = std::make_shared<std::any>(std::any_cast<std::function<std::uint64_t(const std::shared_ptr<MsgType>)>>(get_timestamp));
        timestamp_func_map_.insert(std::make_pair(type_name, func));
    }
   
   template <typename MsgType, typename ...OtherTypeMsg>
  
   
   void reduceTo(int index, std::uint64_t timestamp)
    {
        auto& datas = ds_[index];
        while(datas.size() && (timestamp >= datas.front().first)){
            datas.pop_front();
        }
    }
    virtual void dataready() = 0;

private:
    std::shared_mutex locker_;
    StampFuncTypeMap timestamp_func_map_;
    DataDequeType ds_;
    size_t sync_num_;
    
    std::function<void(const MsgsType&...)> handler_;
};


}
#endif