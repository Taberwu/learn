#ifndef SINSOUT_H
#define SINSOUT_H

#include "platform/Node/include/NodeProduct/LoopFunNode.h"
#include "modules/sp_dr/include/triggers/assist.h"
#include "modules/message/custom_interface/lcm/INERTIAL_Debug.hpp"
#include "modules/message/custom_interface/lcm/VCU_VEHILCE_INFO_After20170612.hpp"

/*******2019-11-25-by-Alex********/
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <eigen3/Eigen/Cholesky>
/*********************************/

#include "cstdlib"

namespace atd {
namespace sp_dr {
/*******2019-11-25-by-Alex********/
using namespace Eigen;
/*********************************/
using atd::common::Status;

using atd::node::LFNode;
using atd::node::GenericShellNode;

const double IMUrate = 100.0;
const double Re = 6378135.072;
const double e=1.0/298.257223563;
const double g0=9.7803267714;
const double gk1=0.00193185138639;
const double gk2=0.00669437999013;
const double W_ie=0.00007292115147;
const double eps=0.00000000000000022204;

class Strapdowncal
{
public:
    Strapdowncal();
    void Cbt_cal(double fai, double si, double ga);
    void zitai_4(double heading,double pitch,double roll);
    void sita_cal(double delta_time);
    void initialization(CoupleOutput *send_sins_last, SensorDataInput *recv_sins_totle, InternalData  *Internal_sins);
    //void initialization_with_uwb(CoupleOutput *send_sins_last, SensorDataInput *recv_sins_totle, SensorDataInput *recv_uwb_totle,InternalData  *Internal_sins);
    void sinsdata_transfer(SensorDataInput *recv_sins_totle,SensorDataInput *recv_sins_cal);
    void strapdown_calculating(double& lat,double& lon, double& height,double& time,double& heading,const std::shared_ptr<VCU_VEHILCE_INFO> &vcu_raw_msg,const std::shared_ptr<INERTIAL_Debug> &ins_raw_msg);
    void set_vcu_data( const std::shared_ptr<VCU_VEHILCE_INFO> &vcu_raw_msg);
    void set_ins_data( const std::shared_ptr<INERTIAL_Debug> &ins_raw_msg);

    int init_flag;

    /*******2019-11-25-by-Alex********/
    MatrixXd Cbt;
    MatrixXd Ctb;
    MatrixXd wie_n;
    MatrixXd wet_t;
    MatrixXd wit_t;
    MatrixXd wit_b;
    MatrixXd wtb_b;
    MatrixXd zt4;
    MatrixXd sita;
    MatrixXd CbtM;
    MatrixXd CtbM;
    MatrixXd f_INSt;
    MatrixXd acc;
    MatrixXd Vel;
    MatrixXd Vellast;
    MatrixXd eye4;
    MatrixXd cross1;
    MatrixXd gMatrix;
    /*********************************/

private:
    //void set_vcu_data();
    //int set_ins_data();
    //int set_uwb_data();

};




}  // namespace localization
}  // namespace atd
#endif
