/****************************************************************************************
 * @FilePath: surrounding_wheel_calib.cpp
 * @Author: Taber.wu
 * @Date: 2024-03-19 13:38:55
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-04-26 17:09:33
 * @Copyright: 2024 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#if 1
#include <jsslam/calibration/surrounding_wheel_calib.h>
#include <jsslam/algorithm/draw_polygon.h>
#include <jsos/message/sensor_msgs/image_convert.hpp>
#include <manif/SO3.h>
#include <iostream>

namespace jsos{namespace algorithm{

SurroundingWheelCalib::SurroundingWheelCalib(const double& wheel_base, float avm_resize, bool manual_slot):
    wheel_base_(wheel_base), avm_resize_(avm_resize), calib_finished_(false), manual_slot_(manual_slot), dbg_last_stamp_(0)
{
    nn_search_ = std::make_shared<NearestNeighborsIndexing>(4);
    calib_err_ = nullptr;
    out_.str("");
    ext_rot_ = Eigen::Quaterniond::Identity();
    ext_trans_ = Eigen::Vector3d::Zero();
    slot_image_cnt_ = 0;
}

void SurroundingWheelCalib::insert(const std::shared_ptr<sensor_msgs::Objects>& object){
    auto stamp = object->header().timestamp();
    if((!slot_calibers_.empty()) &&(stamp <= slot_calibers_.back()->timestamp)){
        return;
    }
    auto slot = std::make_shared<ParkingSlotCalib>();
    slot->timestamp = stamp;
    int id = 0;
    for(auto &park :object->parking_slots()){
        // auto id = park.id();
        auto p0 = park.points(0);
        auto p1 = park.points(1);
        float factor[5] = {0.f};
        factor[0] = 0.5 * (p0.x() + p1.x());
        factor[1] = 0.5 * (p0.y() + p1.y());
        double theta = atan2(p1.y() - p0.y(), p1.x() - p0.x());
        factor[2] = cos(theta);
        factor[3] = sin(theta);
        factor[4] = std::sqrt((p1.x() -p0.x())*(p1.x()-p0.x()) + (p1.y()-p0.y())*(p1.y()-p0.y()));
        slot->slots_center.insert({id, SlotCenterFactor(factor)});
        id++;
    }
    cv::Mat raw_avm;
    jsos::sensor_msgs::toCv(object->avm(), raw_avm);
    jsos::algorithm::graying(raw_avm.clone(), slot->image);
    if(std::abs(avm_resize_ - 1.) > std::numeric_limits<float>::epsilon()){
        cv::resize(slot->image, slot->image, cv::Size(slot->image.cols * avm_resize_, slot->image.rows * avm_resize_));
    }
    slot_calibers_.emplace_back(slot);
    calib_finished_ = false;
}

void SurroundingWheelCalib::insert(const std::shared_ptr<sensor_msgs::Wheel>& wheel){
    auto stamp = wheel->header().timestamp();
    if((!wheel_calibers_.empty()) && (stamp <= wheel_calibers_.back()->timestamp)){
        return;
    }
    auto w = std::make_shared<WheelCalib>();
    w->timestamp = stamp;
    if(wheel->type() == sensor_msgs::Wheel::WheelTypeVXYW){
        w->wheel = Eigen::Vector3d(wheel->vx(), wheel->vy(), wheel->vw());
    }
    else if(wheel->type() == sensor_msgs::Wheel::WheelTypeCarLike){
        double vx = (wheel->vrl() + wheel->vrr()) / 2.;
        double omega = (wheel->vrr() - wheel->vrl()) / wheel_base_;
        w->wheel = Eigen::Vector3d(vx, 0., omega);
    }
    else{return;}
    wheel_calibers_.emplace_back(w);
    calib_finished_ = false;
}

void SurroundingWheelCalib::remove(const std::vector<size_t>& idx)
{
    for(auto iter = idx.rbegin(); iter != idx.rend(); iter ++){
        slot_calibers_.erase(std::next(slot_calibers_.begin(), *iter));
        wheel_calibers_.erase(std::next(wheel_calibers_.begin(), *iter));
    }
}

void SurroundingWheelCalib::remove(){
    std::vector<size_t> remove_idxs;
    for(size_t pos = 0; pos < slot_calibers_.size(); pos++){
        if(!(slot_calibers_[pos]->enable)){
            remove_idxs.push_back(pos);
        }
    }
    remove(remove_idxs);
}

void SurroundingWheelCalib::extract(){
    printf("start to extract slot size %lu wheel size %lu\n", slot_calibers_.size(), wheel_calibers_.size());
    sync(slot_calibers_, wheel_calibers_);
    printf("after sync slot size %lu wheel size %lu\n", slot_calibers_.size(), wheel_calibers_.size());
    slot_calibers_[0]->tcw = Eigen::Vector3d::Zero();
    slot_calibers_[0]->rcw = Eigen::Quaterniond::Identity();
    std::vector<size_t> remove_idxs; 
    for(size_t pos = 1; pos < slot_calibers_.size(); pos++){
        nn_search_->clear();
        for(auto &park : slot_calibers_[pos]->slots_center){
            nn_search_->insert(park.first, park.second.data());
        }
        auto sts = estimateSlotPos(slot_calibers_[pos-1], slot_calibers_[pos], wheel_calibers_[pos-1], wheel_calibers_[pos], true);
        if(!sts){
            remove_idxs.emplace_back(pos);
        }
    }
    if((remove_idxs.size() + 6 > slot_calibers_.size()) || (remove_idxs.size() > slot_calibers_.size() * 0.5)){
        printf("too many unpos parkslot to estimate \n");
        return;
    }
    for(auto iter = remove_idxs.rbegin(); iter != remove_idxs.rend(); iter ++){
        slot_calibers_.erase(std::next(slot_calibers_.begin(), *iter));
        wheel_calibers_.erase(std::next(wheel_calibers_.begin(), *iter));
    }
    calib_finished_ = false;
}

void SurroundingWheelCalib::calib(){
    ceres::Problem problem;
    ceres::LossFunction* loss_function = new ceres::CauchyLoss(1.0);
    std::vector<double> params(3, 0.);
    if(slot_calibers_.size() != wheel_calibers_.size()){
        printf("slot size != wheel size \n");
        return;
    }
    

    auto last_yaw = jsos::utility::quaternion2EeulerAngle(wheel_calibers_[0]->rcw).z();
    auto last_wheel_pos = wheel_calibers_[0]->tcw;
    int calib_use_cnt = 0;
    for(size_t pos = 1; pos < slot_calibers_.size(); pos++){
        auto cur_wheel_pos =  wheel_calibers_[pos]->tcw;
        auto tra = (cur_wheel_pos - last_wheel_pos).norm();
        if( tra < 0.3) continue;
        double wheel_yaw = jsos::utility::quaternion2EeulerAngle(wheel_calibers_[pos]->rcw).z();
        if(fabs(tra/(wheel_yaw - last_yaw)) > 0.02){
            calib_use_cnt ++;
            auto w_q = wheel_calibers_[pos-1]->rcw.conjugate() * wheel_calibers_[pos]->rcw;
            auto w_t = wheel_calibers_[pos-1]->rcw.conjugate() * (cur_wheel_pos - last_wheel_pos);
            auto s_q = slot_calibers_[pos -1]->rcw.conjugate() * slot_calibers_[pos]->rcw;
            auto s_t = slot_calibers_[pos -1]->rcw.conjugate() * (slot_calibers_[pos]->tcw - slot_calibers_[pos-1]->tcw);
            auto cost_function = SurrWheelCalibCostFunction::generateCostFunction(w_t, w_q, s_t, s_q);
            problem.AddResidualBlock(cost_function, loss_function, params.data());
            printf("calib cnt %d stamp[%lu %lu]------------------\n", calib_use_cnt, slot_calibers_[pos]->timestamp, wheel_calibers_[pos]->timestamp);
            printf("wheel pos [%.5f, %.5f,0.] yaw %.7f\n", w_t.x(), w_t.y(), wheel_yaw *180./M_PI);
            // printf("wheel pos [%.5f, %.5f,0.] yaw %.7f\n", wheel_calibers_[pos]->tcw.x(), wheel_calibers_[pos]->tcw.y(), wheel_yaw *180./M_PI);
            double slot_yaw = jsos::utility::quaternion2EeulerAngle(s_q).z() *180/M_PI;
            // printf("parkslot pos [%.5f, %.5f, 0.] yaw %.7f\n", slot_calibers_[pos]->tcw.x(), slot_calibers_[pos]->tcw.y(), slot_yaw);
            printf("parkslot pos [%.5f, %.5f, 0.] yaw %.7f\n",s_t.x(), s_t.y(), slot_yaw);
            
        }  
        last_yaw = wheel_yaw;
        last_wheel_pos = cur_wheel_pos;
    }
    
    ceres::Solver::Options options;
    options.minimizer_progress_to_stdout = true;
    options.logging_type = ceres::PER_MINIMIZER_ITERATION;
    options.max_num_iterations = 1000;
    options.num_threads = 1;
    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);
    std::cout<<summary.FullReport()<<std::endl;
    ext_trans_ = Eigen::Vector3d(params[0], params[1], 0);
    std::string context;
    context = "Wheel Surrounding Extrinsic Params\n";
    out_ << context;
    context = " Translation (surrouding in wheel)\n";
    context += "    X: "; context += std::to_string(params[4]);context += "\n";
    context += "    Y: "; context += std::to_string(params[5]);context += "\n";
    out_ << context;
    ext_rot_ =  Eigen::Quaterniond(cos(0.5 * params[2]), 0., 0., sin(0.5 * params[2]));
    ext_rot_.normalize();
    context = " Quaternion (surrouding in wheel)\n";
    context += "    W: "; context += std::to_string(ext_rot_.w());context += "\n";
    context += "    X: "; context += std::to_string(ext_rot_.x());context += "\n";
    context += "    Y: "; context += std::to_string(ext_rot_.y());context += "\n";
    context += "    Z: "; context += std::to_string(ext_rot_.z());context += "\n";
    out_ << context;
    context = " RotationMatrix (surrouding in wheel)\n";
    out_ << context;
    out_ << ext_rot_.toRotationMatrix()<<std::endl;
    context = " Euler (surrouding in wheel rad)\n";
    auto euler = jsos::utility::quaternion2EeulerAngle(ext_rot_);
    context += "    Roll: "; context += std::to_string(euler.x());context += "\n";
    context += "    Pitch: "; context += std::to_string(euler.y());context += "\n";
    context += "    Yaw: "; context += std::to_string(euler.z());context += "\n";
    out_ << context;

    calib_err_ = std::make_shared<std::vector<Eigen::Vector3d>>();
    for(size_t i = 0; i < wheel_calibers_.size(); i++){
        Eigen::Vector3d err = Eigen::Vector3d::Zero();
        Eigen::Quaterniond cal_q(params[3], params[0], params[1], params[2]);
        Eigen::Vector3d cal_t (params[4], params[5], 0.);
        auto park_theta = jsos::utility::quaternion2EeulerAngle(slot_calibers_[i]->rcw).z();
        auto wheel_theta = jsos::utility::quaternion2EeulerAngle(wheel_calibers_[i]->rcw).z();
        auto cal_delta_theta = jsos::utility::quaternion2EeulerAngle(cal_q).z();
        auto cal_wheel = cal_q.conjugate() * slot_calibers_[i]->tcw;
        cal_wheel = cal_wheel + cal_t;
        auto l = wheel_calibers_[i]->rcw.conjugate() * cal_t;
        cal_wheel = cal_wheel -l;
        //  (cal_t + cal_q.conjugate() * slot_calibers_[i]->tcw ) - (wheel_calibers_[i]->rcw.conjugate() * cal_t) ;
        
        err.x() = wheel_calibers_[i]->tcw.x() - cal_wheel.x();
        err.y() = wheel_calibers_[i]->tcw.y() - cal_wheel.y();
        err.z() = (cal_delta_theta - (park_theta - wheel_theta)) *180/M_PI;
        calib_err_->emplace_back(err);
    }
    calib_finished_ = true;
}

std::string SurroundingWheelCalib::output(){
    return out_.str();
}

std::shared_ptr<std::vector<Eigen::Vector3d>> SurroundingWheelCalib::showCalibRes(std::vector<Eigen::Vector3d>& wheel_tra, std::vector<Eigen::Vector3d>& slot_tra){
    wheel_tra.clear();
    slot_tra.clear();
    if(!calib_finished_) return nullptr;
    wheel_tra.reserve(wheel_calibers_.size());
    slot_tra.reserve(wheel_calibers_.size());
    for(size_t i = 0; i < wheel_calibers_.size(); i++){
        auto &wheel = wheel_calibers_[i];
        double wheel_yaw = jsos::utility::quaternion2EeulerAngle(wheel->rcw).z();
        wheel_tra.emplace_back(Eigen::Vector3d(wheel->tcw.x(), wheel->tcw.y(), wheel_yaw));
        auto &slot = slot_calibers_[i];
        double slot_yaw = jsos::utility::quaternion2EeulerAngle(slot->rcw).z();
        slot_tra.emplace_back(Eigen::Vector3d(slot->tcw.x(), slot->tcw.y(), slot_yaw));
    }

    return calib_err_;
}

void SurroundingWheelCalib::draw(int index, cv::Mat& image){
    printf("stamp %.9f", (double)(slot_calibers_[index]->timestamp * 1e-9));
    image = slot_calibers_[index]->image.clone();
    cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);
    cv::Scalar measure_slot_color = cv::Scalar(0, 255, 255);
    drawSlot(slot_calibers_[index]->slots_center, measure_slot_color, image);
    if(calib_finished_){
        cv::Scalar predict_slot_color = cv::Scalar(0, 97, 255);
        drawSlot(slot_calibers_[index]->predict_slots_center, predict_slot_color, image);
    }
    
}

bool SurroundingWheelCalib::estimateSlotPos(const std::shared_ptr<ParkingSlotCalib>& last_slot,  std::shared_ptr<ParkingSlotCalib>& curr_slot, 
            std::shared_ptr<WheelCalib> last_wheel, std::shared_ptr<WheelCalib> wheel, bool usewheel_predict){
    Eigen::Vector3d last_t = last_slot->tcw;
    Eigen::Quaterniond last_q = last_slot->rcw;
    Eigen::Quaterniond to_cur_q = Eigen::Quaterniond::Identity();
    Eigen::Vector3d to_cur_t = Eigen::Vector3d::Zero();
    std::map<std::int64_t, std::pair<SlotCenterFactor, float>> match_pts;
    float min_distance = 10000.f;
    if(manual_slot_){
        for(auto cur_park : curr_slot->slots_center){
            if(last_slot->slots_center.find(cur_park.first) != last_slot->slots_center.end()){
                match_pts[cur_park.first] = std::make_pair(last_slot->slots_center[cur_park.first], 0.f);
                // printf("match cur[%.5f %.5f] last[%.5f %.5f]\n", cur_park.second.x(), cur_park.second.y(), last_slot->slots_center[cur_park.first].x(), last_slot->slots_center[cur_park.first].y());
            }
        }
        min_distance = 0.f;
    }
    else{
        SlotsCenter predict_slot;
        if(usewheel_predict){
           to_cur_q = wheel->rcw * last_wheel->rcw.conjugate();
           to_cur_q.normalize();
           to_cur_t = (to_cur_q * last_wheel->tcw) - wheel->tcw;
           predictSlotPoints(last_slot, to_cur_q, to_cur_t, predict_slot);
        }
        else{
            predict_slot = last_slot->slots_center;
        } 
    
        for(auto pt : predict_slot){
            std::vector<std::int64_t> idx(2, -1);
            std::vector<float> distance(2, 1.);
            nn_search_->search(pt.second.data(), 2, idx, distance);
            if(distance[0] < distance[1] * 0.6){
                auto last_pt = last_slot->slots_center[pt.first];
                auto cur_factor = curr_slot->slots_center[idx[0]];
                if((match_pts.find(idx[0]) ==  match_pts.end())  ||(match_pts[idx[0]].second > distance[0])){
                    match_pts[idx[0]] = std::make_pair(last_pt, distance[0]);
                    // printf("current [%.3f %.3f]  predict [%.3f %.3f] last [%.3f %.3f]   distance %.7f\n", cur_factor.x(), cur_factor.y(), pt.second.x(), pt.second.y(), last_pt.x(), last_pt.y(), distance[0]);
                    min_distance = (min_distance > distance[0])? distance[0]: min_distance;
                }
            }
        }
    }
    if((match_pts.size() < 1 ) || (min_distance > 6.f)){ 
            printf("###########fail to estimate slot pos use wheel predict\n");
            //use wheel predict
            curr_slot->rcw = (wheel->rcw * last_wheel->rcw.conjugate()) * last_q;
            curr_slot->tcw = last_t + (wheel->tcw - last_wheel->tcw);
            return false;
    }
    std::vector<double> params(3, 0.);
    ceres::Problem problem;
    ceres::LossFunction* loss_function = new ceres::CauchyLoss(1.0);
    for(auto& mth : match_pts){
        auto cost_fun = SlotMatchCostFunction::generateCostFunction(curr_slot->slots_center[mth.first], mth.second.first);
        problem.AddResidualBlock(cost_fun, loss_function, params.data());
    }
    ceres::Solver::Options options;
    options.minimizer_progress_to_stdout = false;
    options.logging_type = ceres::PER_MINIMIZER_ITERATION;
    options.max_num_iterations = 1000;
    options.num_threads = 1;
    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);
    to_cur_q = Eigen::Quaterniond(cos(0.5 * params[0]), 0., 0., sin(0.5 * params[0]));
    to_cur_q.normalize();
    to_cur_t = Eigen::Vector3d(params[1], params[2], 0.);
    curr_slot->rcw = to_cur_q.conjugate() * last_q;
    curr_slot->tcw = last_t - curr_slot->rcw * to_cur_t;
    // printf("stamp %lu\n", curr_slot->timestamp);
    // printf("l2c q[ %.7f] t[%.5f %.5f]\n", params[0] * 180./M_PI, params[1], params[2]);
    // printf("pos [%.5f %.5f]\n", curr_slot->tcw.x(), curr_slot->tcw.y());
    // printf("----------------\n");
    for(auto& pt : last_slot->slots_center){
        double d_theta = jsos::utility::quaternion2EeulerAngle(to_cur_q).z();
        double last_theta = atan2(pt.second.sinx(), pt.second.cosx());
        double theta = last_theta + d_theta;
        Eigen::Vector3d center(pt.second.x(), pt.second.y(), 0.);
        center = to_cur_q * center + to_cur_t;
        curr_slot->predict_slots_center.insert(std::make_pair(pt.first, SlotCenterFactor(float(center.x()), float(center.y()), float(cos(theta)), float(sin(theta)), pt.second.width())));
    }
    return true;
}

void SurroundingWheelCalib::sync(std::deque<std::shared_ptr<ParkingSlotCalib>> &slot_calibers,  std::deque<std::shared_ptr<WheelCalib>> &wheel_calibers){
    Eigen::Vector3d last_tcw = Eigen::Vector3d::Zero();
    Eigen::Quaterniond last_rcw = Eigen::Quaterniond::Identity();
    while((!slot_calibers.empty()) && (slot_calibers.front()->timestamp < wheel_calibers.front()->timestamp)){
        slot_calibers.pop_front();
    }
    std::deque<std::shared_ptr<WheelCalib>> synced_wheels;
    auto wheel_iter = wheel_calibers.begin();
    while((std::next(wheel_iter) != wheel_calibers.end()) && ((*std::next(wheel_iter))->timestamp <= slot_calibers.front()->timestamp)){
        wheel_iter++;
    }
    auto sync_wheel = std::make_shared<WheelCalib>();
    sync_wheel->timestamp = slot_calibers.front()->timestamp;
    sync_wheel->wheel = (*wheel_iter)->wheel;
    sync_wheel->tcw = last_tcw;
    sync_wheel->rcw = last_rcw;
    synced_wheels.emplace_back(sync_wheel);
    for(size_t idx = 1; idx < slot_calibers.size(); idx++){
        if(wheel_iter == wheel_calibers.end()){
            slot_calibers.erase(slot_calibers.begin()+idx, slot_calibers.end());
            break;
        }
        auto slot_calib = slot_calibers[idx];
        auto last_stamp = (synced_wheels.back())->timestamp;
        // printf("wheel stamps[ %lu ", last_stamp);
        double dt_sum = 0., dyaw_sum = 0.;
        while((std::next(wheel_iter) != wheel_calibers.end()) && ((*std::next(wheel_iter))->timestamp <= slot_calib->timestamp)){
            auto next_wheel = std::next(wheel_iter);
            double dt = (static_cast<double>((*next_wheel)->timestamp) - static_cast<double>(last_stamp)) * 1e-9;
            double m_vx = 0.5 * ((*next_wheel)->wheel.x() + (*wheel_iter)->wheel.x());
            double m_vw = (*next_wheel)->wheel.z();
            last_stamp = (*next_wheel)->timestamp;
            dt_sum += dt;
            dyaw_sum += m_vw * dt;
            updateWheelPos(m_vx, m_vw, dt, last_tcw, last_rcw);
            wheel_iter ++;
        }
        
        double dt = (static_cast<double>(slot_calib->timestamp) - static_cast<double>((*wheel_iter)->timestamp)) * 1e-9;
        dt_sum += dt;
        dyaw_sum += (*wheel_iter)->wheel.z() * dt;
        // printf("%lu]\n", slot_calib->timestamp);
        // printf("slot park %lu   wheel dt %.5f   dyaw %.7f\n", slot_calib->timestamp, dt_sum, dyaw_sum);
        updateWheelPos((*wheel_iter)->wheel.x(), (*wheel_iter)->wheel.z(), dt, last_tcw, last_rcw);
        auto sync_wheel = std::make_shared<WheelCalib>();
        sync_wheel->timestamp = slot_calib->timestamp;
        sync_wheel->wheel = (*wheel_iter)->wheel;
        sync_wheel->tcw = last_tcw;
        sync_wheel->rcw = last_rcw;
        synced_wheels.emplace_back(sync_wheel);
    }
    std::swap(synced_wheels, wheel_calibers);
}

void SurroundingWheelCalib::updateWheelPos(const double vx, const double vw, const double dt, Eigen::Vector3d &t, Eigen::Quaterniond &r){
    if(fabs(vx) < 1e-3) return;
    double theta = jsos::utility::quaternion2EeulerAngle(r).z();
    if(fabs(vw) < 1e-4){
        t.x() += vx * cos(theta) * dt;
        t.y() += vx * sin(theta) * dt;
    }
    else{
        t.x() += vx / vw *(sin(theta + vw * dt) - sin(theta));
        t.y() += vx / vw *(cos(theta) - cos(theta + vw * dt));
        r = jsos::utility::eulerAngle2Quaternion(Eigen::Vector3d(0., 0., theta + vw *dt));
        r.normalize();
    }
}

 void SurroundingWheelCalib::predictSlotPoints(const std::shared_ptr<ParkingSlotCalib>& slot, const Eigen::Quaterniond &to_cur_q, 
                const Eigen::Vector3d &to_cur_t, SlotsCenter &p_slot_pts)
{
        p_slot_pts.clear();
        for(auto park : slot->slots_center){
            auto pt = park.second;
            double theta = atan2(pt.sinx(), pt.cosx());
            double delta_theta = jsos::utility::quaternion2EeulerAngle(to_cur_q).z();
            float factor[5] = {0.};
            factor[2] = sin(theta + delta_theta);
            factor[3] = cos(theta + delta_theta);
            Eigen::Vector3d last_p(pt.x(), pt.y(), 0.);
            last_p = to_cur_q * last_p + to_cur_t;
            factor[0] = float(last_p.x());
            factor[1] = float(last_p.y());
            factor[4] = pt.width();
            p_slot_pts.insert({park.first, SlotCenterFactor(factor)});
        }
}

void SurroundingWheelCalib::getTimestamps(std::vector<std::uint64_t> &timestamps){
    timestamps.clear();
    timestamps.reserve(slot_calibers_.size());
    for(auto slot : slot_calibers_){
        timestamps.emplace_back(slot->timestamp);
    }
}

 void SurroundingWheelCalib::drawSlot(const SlotsCenter& slot_centers, const cv::Scalar color, cv::Mat& image ,float depth)
 {
    std::vector<std::vector<cv::Point>> coners;
    coners.reserve(slot_centers.size());
    // printf(" park size %lu \n", slot_centers.size());
    std::vector<cv::Scalar> colors(slot_centers.size(), color);
    float pix2world = 0.025; int offset = 256;
    for(auto slot_center : slot_centers){
        cv::Point p0, p1, p2, p3;
        p0.y = (offset - (slot_center.second.x() - slot_center.second.cosx() * slot_center.second.width()/2.0)/pix2world) * avm_resize_;
        p0.x = (offset - (slot_center.second.y() - slot_center.second.sinx() * slot_center.second.width()/2.0)/pix2world) * avm_resize_;
        p1.y = (offset - (slot_center.second.x() + slot_center.second.cosx() * slot_center.second.width()/2.0)/pix2world) * avm_resize_;
        p1.x = (offset - (slot_center.second.y() + slot_center.second.sinx() * slot_center.second.width()/2.0)/pix2world) * avm_resize_;
        // p2.y = (offset - (p1.x - depth * slot_center.second.sinx())/pix2world) * avm_resize_;
        // p2.x = (offset - (p1.y - depth * slot_center.second.cosx())/pix2world) * avm_resize_;
        // p3.y = (offset - (p0.x - depth * slot_center.second.sinx())/pix2world) * avm_resize_;
        // p3.x = (offset - (p0.y - depth * slot_center.second.cosx())/pix2world) * avm_resize_;
        std::vector<cv::Point> coner = {p0, p1};
        // printf("id %d p0[%d %d] p1[%d %d]\n", slot_center.first, p0.x, p0.y, coner[1].x, coner[1].y);
        coners.emplace_back(coner);
    }
    draw_polygon(image, coners, colors);
    printf("---------------------------\n");
 }

 void SurroundingWheelCalib::readManualSlot(const std::string &filename){
        slot_calibers_.clear();
        float pix2world = 0.025;
        int offset = 256;
        std::ifstream config_file(filename);
        auto root = nlohmann::json::parse(config_file, nullptr, true, true);
        auto select_slot = root["select_slot"];
        for(auto park_slot : select_slot){
            auto slot_caliber = std::make_shared<ParkingSlotCalib>();
            slot_caliber->timestamp = std::uint64_t(park_slot["timestamp"].get<double>() * 1e9);
            // printf("stamp %lu\n", slot_caliber->timestamp);
            auto parks = park_slot["parks"];
            for(auto park : parks){
                int id = park["id"].get<int>();
                float p0y = (offset - park["p0x"].get<int>()) * pix2world;
                float p0x = (offset - park["p0y"].get<int>()) * pix2world;
                float p1y = (offset - park["p1x"].get<int>()) * pix2world;
                float p1x = (offset - park["p1y"].get<int>()) * pix2world;
                // printf("id %d p0[%.3f, %.3f] p1[%.3f %.3f]\n", id, p0x, p0y, p1x, p1y);
                float factor[5] = {0.f};
                factor[0] = 0.5 * (p0x + p1x);
                factor[1] = 0.5 * (p0y + p1y);
                double theta = atan2((p1y-p0y), (p1x-p0x));
                factor[2] = cos(theta);
                factor[3] = sin(theta);
                factor[4] = sqrt((p1y-p0y)*(p1y-p0y) + (p1x-p0x)*(p1x-p0x));
                slot_caliber->slots_center.insert(std::make_pair(id, SlotCenterFactor(factor)));
            }
            slot_calibers_.emplace_back(slot_caliber);
        }
        printf("read %lu capture\n", slot_calibers_.size());
 }

 void SurroundingWheelCalib::readImage(const std::shared_ptr<sensor_msgs::Objects>& objects){
    if(slot_image_cnt_ > slot_calibers_.size() -1) return;
    auto record_stamp = slot_calibers_[slot_image_cnt_]->timestamp;
    if(fabs((double)objects->header().timestamp() - (double)record_stamp) < 3e6 ){
        cv::Mat raw_avm;
        jsos::sensor_msgs::toCv(objects->avm(), raw_avm);
        jsos::algorithm::graying(raw_avm.clone(),  slot_calibers_[slot_image_cnt_]->image);
        if(std::abs(avm_resize_ - 1.) > std::numeric_limits<float>::epsilon()){
            cv::resize( slot_calibers_[slot_image_cnt_]->image,  slot_calibers_[slot_image_cnt_]->image, cv::Size( slot_calibers_[slot_image_cnt_]->image.cols * avm_resize_,  slot_calibers_[slot_image_cnt_]->image.rows * avm_resize_));
        }
        slot_image_cnt_++;    
    }
 }


}}

#endif

