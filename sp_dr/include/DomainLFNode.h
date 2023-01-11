#ifndef MODULES_SP_DR_DOMAINLFNODE_H_
#define MODULES_SP_DR_DOMAINLFNODE_H_

#include "platform/Node/include/NodeProduct/LoopFunNode.h"
#include "platform/Node/include/BasicFun/issuecodebase.h"

#include "platform/WorldModel/include/Function/tf/EgoFrame/pdef_ecartesian_conversion.h"
#include "platform/WorldModel/include/Function/tf/InterFrame/wcartesian_ecartesian_conversion.h"
#include "platform/WorldModel/include/Function/tf/WorldFrame/wcartesian_geodetic_conversion.h"
#include "platform/WorldModel/include/Concept/shell/timing.h"

#include "modules/message/drivers/driver_chassis.h"
#include "modules/message/drivers/driver_ins.h"
#include "modules/message/drivers/driver_insdebug.h"

#include "platform/Scenario/include/Concept/obstacle/env_velocity.h"
#include "platform/Scenario/include/Concept/obstacle/env_dr.h"
#include "platform/Scenario/include/Concept/env_pose.h"

#include "modules/sp_dr/include/triggers/vcu_dr.h"
#include "modules/sp_dr/include/triggers/sinsout.h"
/*******2019-11-25-by-Alex********/
#include "modules/sp_dr/include/triggers/assist.h"
#include "platform/WorldModel/include/Concept/entity.h"
#include "platform/Common/include/time.h"
/*********************************/

namespace atd {
namespace sp_dr {

using atd::common::Status;
using namespace atd::node;
using namespace atd::worldmodel;
using namespace atd::scenario;

//LFNode Main Function
class DomainLFNode:public LFNode,public IssueCodeBase
{
public:
    DomainLFNode():LFNode(),gps_raw_list(50),vcuraw_list(50),imu_raw_list(50)
    {

    }
    virtual ~DomainLFNode() = default;

    std::string ExportDebugString() const override
    {
        return "none";
    }

    Status Init() override;

    Status Step() override;

private:

//    DriverIns ins_;
//    DriverINERTIAL_Debug ins_debug_;
//    DriverChassis chassis_;
//    std::shared_ptr<VCU_VEHILCE_INFO> inner_vcu;

//    std::shared_ptr<INERTIAL_Debug> inner_inertial_debug;

    WorldModelParse<atd::node::pb_p_mutiarrow,EnvParse_EnvDr> parse_dr_;
    EnvDr dr_;

    WorldModelParse<atd::node::pb_p_mutiarrow,EnvParse_EnvDr> parse_dr_utm_;
    EnvDr dr_utm_;

    WorldModelParse<atd::node::pb_p_mutiarrow,EnvParse_EnvDr> parse_dr_for_high_;
    EnvDr dr_for_high_;

    Status MergeDr_from_raw_gps(const Arrow2d &gps, const double timestamp_sec, Timing<Arrow2d> *result,Timing<Arrow2d> *result_utm);
    Status MergeDr_from_vcu(const float velocity_mps,const float fSteeringAngle,
                            const int gear,const double timestamp_sec,Timing<Arrow2d>* result);
    Status MergeDr_from_raw_imu(const Arrow2d &gps, const double timestamp_sec, Timing<Arrow2d> *result);
    Status PredStage(Timing<Arrow2d> *result,const float fspeed);
    Status PredStage_UTM(Timing<Arrow2d> *result,const float fspeed);

    TimingHistory<Arrow2d> gps_raw_list;

    Timing<Arrow2d> dr_list_vcu;
    TimingHistory<Arrow2d> vcuraw_list;

    TimingHistory<Arrow2d> imu_raw_list;

    VCUdr vcu_method_;
    Strapdowncal ins_strapdown;

    WorldModelParse<atd::node::pb_p_arrow3d,EnvParse_WorldFrame_EnvPose> parse_pose_utm_;
    EnvPose pose_utm_;

    //method
    WcartesianEcartesianConverter wec;
    PdefEcartesianConverter pec;
    WcartesianGeodeticConverter wgc;

};


}
}

#endif  //MODULES_SP_DR_DOMAINLFNODE_H_
