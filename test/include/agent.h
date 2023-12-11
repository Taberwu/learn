#pragma once
#include <memory>
#include <string>
#include <functional>
#include <jsos/utility/rotation.hpp>
#include <jsos/message/sensor_msgs/imu.pb.h>
#include <jsos/message/sensor_msgs/solved_gnss.pb.h>
#include <jsos/message/sensor_msgs/inspva.pb.h>
#include <jsos/message/sensor_msgs/wheel.pb.h>
#include <jsos/message/slam_msgs/localization.pb.h>

namespace jsos{namespace algorithm{
class Applet;
}}

namespace jsos{namespace algorithm{namespace slam{
class SnapshotProvider;
class SolverHandler;

class Agent{
public:
    Agent(const std::string& config);//定位主线程，读取缓存deque进行积分，并丢弃已使用的历史数据，定周期 通过触发 registerTransformHandler registerGnssHandler registerInspvaHandler三个回调函数发布定位数据
    virtual ~Agent();
public:
    void sensory(std::shared_ptr<sensor_msgs::Imu> message);//输入 IMU数据 缓存到deque 加锁
    void sensory(std::shared_ptr<sensor_msgs::SolvedGnss> message);
    void sensory(std::shared_ptr<sensor_msgs::Inspva> message);//输入 卫星导航数据 缓存到deque 加锁
    void sensory(std::shared_ptr<sensor_msgs::Wheel> message);//输入 底盘轮速数据 缓存到deque 加锁
    //void sensory(std::shared_ptr<message::PointCloudMessage> message);
    //void sensory(std::shared_ptr<message::HDMapMessage> message);
    //void sensory(std::shared_ptr<message::ObjectsMessage> message);
    bool registerTransformHandler(const std::function<void(const std::shared_ptr<slam_msgs::Localization>)>& handler);
    bool registerGnssHandler(const std::function<void(const std::shared_ptr<slam_msgs::Localization>)>& handler);
    bool registerInspvaHandler(const std::function<void(const std::shared_ptr<slam_msgs::Localization>)>& handler);
protected:
    std::shared_ptr<SnapshotProvider> provider_;
    std::shared_ptr<SolverHandler> handler_;
    std::shared_ptr<jsos::algorithm::Applet> applet_;
};

}}}

