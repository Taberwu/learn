#include "modules/sp_dr/include/DomainLFNode.h"

#include "modules/sp_dr/include/triggers/vcu_dr.h"
#include "platform/WorldModel/include/Concept/geo/linear_interpolation.h"

//============mainloader start=================
#ifndef STATIC_COMPILE
#include "platform/Node/include/load_node.h"
REGISTER_LFNODE(sp_dr);
#endif
//============mainloader end=================

namespace atd {
namespace sp_dr {

using google::protobuf::StrCat;
using atd::common::Status;

using atd::worldmodel::lerp;
using atd::worldmodel::Angle32;


Status DomainLFNode::MergeDr_from_raw_gps(const Arrow2d& gps,const double timestamp_sec,Timing<Arrow2d>* result,Timing<Arrow2d>* result_utm)
{
    gps_raw_list.PushHistoryData(gps,timestamp_sec);

    //trans data
    result->ClearData();
    result_utm->ClearData();
    for(auto iter=gps_raw_list.begin();iter!=gps_raw_list.end();iter++)
    {
        wec.Set_OriginPoint(gps_raw_list.get_latest_observed()->get_entity());
        Arrow2d tmpvec;
        wec.UTM2EC_Vector(iter->get_entity(),&tmpvec);

        TimeClip<Arrow2d> tmp_clip;
        tmp_clip.set_count_gap(iter->get_count_gap());
        tmp_clip.set_timestamp_sec(iter->get_timestamp_sec());
        tmp_clip.set_entity(tmpvec);
        result->AddCalculatedHistoryData(tmp_clip);

//       std::cout<<COUT_BLUE<<"dr_utm timestamp is:"<<std::to_string(iter->get_timestamp_sec())<<" dr_utm_x is:"<<std::to_string(iter->get_entity().center_x())<<" dr_utm_y is:"<<std::to_string(iter->get_entity().center_y())<<"  dr heading is:"<<std::to_string(iter->get_entity().heading())<<std::endl;


       TimeClip<Arrow2d> tmp_clip_utm;
        tmp_clip_utm.set_count_gap(iter->get_count_gap());
        tmp_clip_utm.set_timestamp_sec(iter->get_timestamp_sec());
        tmp_clip_utm.set_entity(iter->get_entity());
        //std::cout<<"tmp_clip_utm"<<tmp_clip_utm.get_entity().center_x()<<tmp_clip_utm.get_entity().center_y()<<tmp_clip_utm.get_entity().heading()<<std::endl;
        result_utm->AddCalculatedHistoryData(tmp_clip_utm);

    }

    RETURN_STATUS_OK();
}

int imu_raw_dr_count=0;

Status DomainLFNode::MergeDr_from_raw_imu(const Arrow2d& gps,const double timestamp_sec,Timing<Arrow2d>* result)
{
    if(imu_raw_dr_count>10)
    {
        imu_raw_list.PushHistoryData(gps,timestamp_sec);
        imu_raw_dr_count=0;
    }
    else
    {
        imu_raw_dr_count++;
    }

    // std::cout<<"imu_raw_dr_count="<<imu_raw_dr_count<<std::endl;

    //trans data
    result->ClearData();
    //result_utm->ClearData();
    for(auto iter=imu_raw_list.begin();iter!=imu_raw_list.end();iter++)
    {
        wec.Set_OriginPoint(imu_raw_list.get_latest_observed()->get_entity());
        Arrow2d tmpvec;
        wec.UTM2EC_Vector(iter->get_entity(),&tmpvec);
        //    fprintf(stderr,"imu_dr=%f,%f\n",iter->get_entity().center_x(),iter->get_entity().center_y());
        TimeClip<Arrow2d> tmp_clip;
        tmp_clip.set_count_gap(iter->get_count_gap());
        tmp_clip.set_timestamp_sec(iter->get_timestamp_sec());
        tmp_clip.set_entity(tmpvec);
        result->AddCalculatedHistoryData(tmp_clip);

        //        TimeClip<Arrow2d> tmp_clip_utm;
        //        tmp_clip_utm.set_count_gap(iter->get_count_gap());
        //        tmp_clip_utm.set_timestamp_sec(iter->get_timestamp_sec());
        //        tmp_clip_utm.set_entity(iter->get_entity());
        //        result_utm->AddCalculatedHistoryData(tmp_clip_utm);

    }

    RETURN_STATUS_OK();
}

Status DomainLFNode::MergeDr_from_vcu(const float velocity_mps,const float fSteeringAngle,
                                      const int gear,const double timestamp_sec,Timing<Arrow2d>* result)
{
    Arrow2d xy;
    float diff=0;
    if(vcuraw_list.get_data_queue().size()>0)
    {
        if(floor(timestamp_sec*1000)!=floor(vcuraw_list.begin()->get_timestamp_sec()*1000))
        {
            diff=(timestamp_sec-vcuraw_list.begin()->get_timestamp_sec());
            //std::cout<<diff<<std::endl;
            vcu_method_.dr_process(velocity_mps,fSteeringAngle,gear,diff,xy);

            //reset the system
            double timestamp_gap=fabs(diff);
            if(timestamp_gap>2)
            {
                diff=0;
                vcuraw_list.ClearData();
            }
            //std::cout<<xy.ExportDebugString()<<std::endl;
        }
    }

    //update the previous point;
    pec.set_vector(xy);
    for(auto iter=vcuraw_list.mutable_data_queue()->begin();iter!=vcuraw_list.mutable_data_queue()->end();iter++)
    {
        Arrow2d tmpvec;
        pec.PEC2CEC_Vector(iter->get_entity(),&tmpvec);
        iter->set_entity(tmpvec);
    }

    //add new point
    vcuraw_list.PushHistoryData(Arrow2d(0,0,0),timestamp_sec);
    //std::cout<<vcuraw_list->ExportDebugString()<<std::endl;


    result->ClearData();
    for(auto iter=vcuraw_list.begin();iter!=vcuraw_list.end();iter++)
    {
        result->AddCalculatedHistoryData(*iter);
    }

    RETURN_STATUS_OK();
}


Status DomainLFNode::PredStage(Timing<Arrow2d> *result,const float fspeed)
{
    //    auto proto_coefficant_msg=GetDataManager()->getDataPtr<atd::node::pb_p_mutiarrow>("CoefficantDebug");
    //    proto_coefficant_msg->ValidCount(2,proto_coefficant_msg->GetData()->header().timestamp_sec());
    //    if(proto_coefficant_msg->get_valid())
    //    {
    //        for(int i=0;i<proto_coefficant_msg->GetData()->object().objects_size();i++)
    //        {
    //            int pred_millsecond=(i+1)*10;
    //            Ar tmp_ar;
    //            Arrow2d tmp_arrow;
    //            proto2wm::from_p_arrow3d(proto_coefficant_msg->GetData()->object().objects(i),&tmp_arrow);
    //            tmp_ar.set_geo_arrow2d(tmp_arrow);
    //            result.AddCalculatedPredictData(tmp_ar,pred_millsecond);
    //        }
    //    }
    //    else
    //    {
    double v=fspeed;
    for(int i=1;i<11;i++)
    {
        int pred_millsecond=i*10;
        result->AddCalculatedPredictData(Arrow2d(v*pred_millsecond/1000,0,0),pred_millsecond);
    }
    //}
    RETURN_STATUS_OK();
}

Status DomainLFNode::PredStage_UTM(Timing<Arrow2d> *result,const float fspeed)
{
    double v=fspeed;
    for(int i=1;i<11;i++)
    {

        int pred_millsecond=i*10;
        wec.Set_OriginPoint(result->get_current_observed()->get_entity());

        Arrow2d tmp_vec;
        tmp_vec.set_center_x(v*pred_millsecond/1000);
        tmp_vec.set_center_y(0);
        tmp_vec.set_heading(0);
        Arrow2d tmp_utm;
        wec.EC2UTM_Vector(tmp_vec,&tmp_utm);

        result->AddCalculatedPredictData(tmp_utm,pred_millsecond);
    }
    RETURN_STATUS_OK();
}


Status DomainLFNode::Init()
{
    std::cout<<"init:"<<get_name()<<" config_url:"<<GetConfigUrl()<<std::endl;

    if(GetConfigUrl()=="dr_vcu")
        std::cout<<COUT_YELLOW<<"cur mode:dr_vcu (using steerangle and speed"<<COUT_RESET<<std::endl;
    else if(GetConfigUrl()=="dr_ins")
        std::cout<<COUT_YELLOW<<"cur mode:dr_ins (using ins_debug raw data and vcu speed to deadreckon)"<<COUT_RESET<<std::endl;
    else
        std::cout<<COUT_YELLOW<<"cur mode:dr_gps (using gps to simu)"<<COUT_RESET<<std::endl;

    SetIssueCodeEnv(get_name(),GetSuperNode());

    parse_dr_.SetDataInterface("SP_Dr_Bottom",GetSuperNode());
    //parse_dr_utm_.SetDataInterface("SP_Dr_Bottom_UTM",GetSuperNode());
    parse_dr_for_high_.SetDataInterface("SP_Dr_For_High",GetSuperNode());
    parse_pose_utm_.SetDataInterface("SP_Dr_Loc_UTM",GetSuperNode());


    RETURN_STATUS_OK();
}


Status DomainLFNode::Step()
{
    //get vcu
    std::shared_ptr<VCU_VEHILCE_INFO> inner_vcu= GetDataManager()->getDataPtr<VCU_VEHILCE_INFO>("INNER_VCU")->GetData();

    //get ins debug
    std::shared_ptr<INERTIAL_Debug> inner_inertial_debug= GetDataManager()->getDataPtr<INERTIAL_Debug>("INNER_INERTIAL_DEBUG")->GetData();

    //std::cout<<"vcu v:"<<inner_vcu->fSpeed<<"ins :"<<inner_inertial_debug->fLatitude<<std::endl;

        if(inner_vcu==nullptr)
        {
             std::cout<<"VCU return"<<std::endl;
            return Status(GenIssueSafeCode_DRIVER(sensor_chassis,"no inner vcu msg"));
        }




        if(inner_inertial_debug==nullptr)
        {
            std::cout<<"interdebug  return"<<std::endl;
            return Status(GenIssueSafeCode_DRIVER(sensor_ins,"no inertial debug msg"));
        }




    double timestamp_start=atd::common::time::ToSecond(atd::common::time::Clock::Now());

    //set cur timestamp
    set_cur_timestamp(inner_inertial_debug->header.nTimeStamp*0.000001);

    if(GetConfigUrl()=="dr_vcu")
    {
        //get chassis
        set_cur_timestamp(inner_vcu->stHeader.nTimeStamp*0.000001);

        MergeDr_from_vcu( inner_vcu->fSpeed,
                          inner_vcu->fSteeringAngle,inner_vcu->nShifterPosition,inner_vcu->stHeader.nTimeStamp*0.000001,
                          &dr_);
    }
    else if(GetConfigUrl()=="dr_ins")
    {

        set_cur_timestamp(inner_inertial_debug->header.nTimeStamp*0.000001);

        double lat,lon,height,heading;
        double gpstime;
        ins_strapdown.strapdown_calculating(lat,lon,height,gpstime,heading,inner_vcu,inner_inertial_debug);

        double utm_x,utm_y,utm_z;
        wgc.GC2UTM(lon,lat,height,&utm_x,&utm_y,&utm_z);
        MergeDr_from_raw_gps(Arrow2d(utm_x,utm_y,heading),gpstime,&dr_,&dr_utm_);

    }
    else
    {

        set_cur_timestamp(inner_inertial_debug->header.nTimeStamp*0.000001);
        double utm_x,utm_y,utm_z;
        wgc.GC2UTM(inner_inertial_debug->fLongitude,inner_inertial_debug->fLatitude,inner_inertial_debug->fAltitude,&utm_x,&utm_y,&utm_z);
        MergeDr_from_raw_gps(Arrow2d(utm_x,utm_y,inner_inertial_debug->fHeading),inner_inertial_debug->header.nTimeStamp*0.000001,&dr_,&dr_utm_);
    }

    //set dr_for_hight========================================
    double lat_,lon_,height_,heading_;
    double gpstime_;
    ins_strapdown.strapdown_calculating(lat_,lon_,height_,gpstime_,heading_,inner_vcu,inner_inertial_debug);

    double utm_x_,utm_y_,utm_z_;
    wgc.GC2UTM(lon_,lat_,height_,&utm_x_,&utm_y_,&utm_z_);

//    std::cout<<COUT_YELLOW<<"lon="<<lon_<<"lat="<<lat_<<"utm_x="<<std::to_string(utm_x_)<<"utm_y="<<std::to_string(utm_y_)<<std::endl;

    MergeDr_from_raw_imu(Arrow2d(utm_x_,utm_y_,heading_),gpstime_,&dr_for_high_);

    Arrow2d inertial_cal_arrow;
    inertial_cal_arrow.set_center_x(utm_x_);
    inertial_cal_arrow.set_center_y(utm_y_);
    inertial_cal_arrow.set_heading(heading_);
    pose_utm_.set_worldframe_observe(inertial_cal_arrow);
    //========================================================

    PredStage(&dr_,inner_vcu->fSpeed);
    parse_dr_.mutable_parser()->set_entity_ptr(&dr_);
    parse_dr_.ComposeAndSendData(get_cur_timestamp());
    //std::cout<<"dr_cyctime:"<<parse_dr_.curnode_cyctime_<<std::endl;
    //std::cout<<"dr_utm"<<dr_utm_.get_oldest_observed()->get_entity().center_x()<<std::endl;
    //PredStage(&dr_utm_);
    //PredStage_UTM(&dr_utm_,inner_vcu->fSpeed);
//    parse_dr_utm_.mutable_parser()->set_entity_ptr(&dr_utm_);
//    parse_dr_utm_.ComposeAndSendData(get_cur_timestamp());

    parse_pose_utm_.mutable_parser()->set_entity_ptr(&pose_utm_);
    parse_pose_utm_.ComposeAndSendData(get_cur_timestamp());
    //PredStage(&dr_for_high_);
    parse_dr_for_high_.mutable_parser()->set_entity_ptr(&dr_for_high_);
    parse_dr_for_high_.ComposeAndSendData(get_cur_timestamp());

    double timestamp_end=atd::common::time::ToSecond(atd::common::time::Clock::Now());
    //GetMonitor()->INFO(GenIssueSafeCode_INFO(info_c0))<<"Cycle Cost time:"<<timestamp_end-timestamp_start;
    //std::cout<<COUT_BOLDCYAN<<"Cycle Cost time:"<<timestamp_end-timestamp_start<<std::endl;

    RETURN_STATUS_OK();
}



}
}

