#include "modules/sp_dr/include/DomainLFNode.h"
#include "platform/WorldModel/include/Concept/geo/angle.h"
#include "modules/sp_dr/include/triggers/sinsout.h"
#include "modules/sp_dr/include/triggers/assist.h"

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <iostream>
#include <math.h>

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <eigen3/Eigen/Cholesky>

namespace atd {
namespace sp_dr {

using atd::common::Status;

using namespace std;
using namespace Eigen;
using atd::worldmodel::Angle32;
using atd::node::GenericShellNode;

//eigen para
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

struct SensorDataInput recv_ins_totle= {0};
//struct SensorDataInput recv_uwb_totle= {0};
struct VCU_data        recv_vcu_totle= {0};
struct CoupleOutput    send_sins_totle= {0};
struct CoupleOutput    send_sins_last_totle= {0};
struct CoupleOutput    send_sins_vcu_totle= {0};
struct CoupleOutput    send_sins_vcu_last_totle= {0};

struct InternalData    Internal_sins_data = {0};
struct SensorDataInput recv_sins_cal_data = {0};

void Strapdowncal::set_vcu_data(const std::shared_ptr<VCU_VEHILCE_INFO> &vcu_raw_msg)
{
    recv_vcu_totle.nShifterPosition=vcu_raw_msg->nShifterPosition;
    recv_vcu_totle.fSteeringAngle=vcu_raw_msg->fSteeringAngle;
    recv_vcu_totle.time=vcu_raw_msg->stHeader.nTimeStamp;
    recv_vcu_totle.fSpeed=vcu_raw_msg->fSpeed;
    recv_vcu_totle.fYawRate=vcu_raw_msg->fYawRate;
    recv_vcu_totle.fFrontLeftWheelSpeed=vcu_raw_msg->fFrontLeftWheelSpeed;
    recv_vcu_totle.fFrontRightWheelSpeed=vcu_raw_msg->fFrontRightWheelSpeed;
    recv_vcu_totle.fRearLeftWheelSpeed=vcu_raw_msg->fRearLeftWheelSpeed;
    recv_vcu_totle.fRearRightWheelSpeed=vcu_raw_msg->fRearRightWheelSpeed;
    recv_vcu_totle.update_flag=1;
    //fprintf(stderr,"set_vcu_data=%f,%f\n",vcu_raw_msg->fSpeed,vcu_raw_msg->fFrontLeftWheelSpeed);
}

void Strapdowncal::set_ins_data(const std::shared_ptr<INERTIAL_Debug> &ins_raw_msg)
{

    recv_ins_totle.longitude=ins_raw_msg->fLongitude;
    recv_ins_totle.latitude=ins_raw_msg->fLatitude;
    recv_ins_totle.height=ins_raw_msg->fAltitude;
    recv_ins_totle.heading=ins_raw_msg->fHeading;
    recv_ins_totle.fPitch=ins_raw_msg->fPitch;
    recv_ins_totle.fRoll=ins_raw_msg->fRoll;
    recv_ins_totle.fVelNorth=ins_raw_msg->fVelNorth;
    recv_ins_totle.fVelWest=ins_raw_msg->fVelWest;
    recv_ins_totle.fVelUp=ins_raw_msg->fVelUp;
    recv_ins_totle.fAccX=ins_raw_msg->fAccX;
    recv_ins_totle.fAccY=ins_raw_msg->fAccY;
    recv_ins_totle.fAccZ=ins_raw_msg->fAccZ;
    recv_ins_totle.fAngRateX=ins_raw_msg->fAngRateX;
    recv_ins_totle.fAngRateY=ins_raw_msg->fAngRateY;
    recv_ins_totle.fAngRateZ=ins_raw_msg->fAngRateZ;

    recv_ins_totle.time=ins_raw_msg->header.nTimeStamp*0.000001;
    // fprintf(stderr,"recv_ins_time=%f\n",recv_ins_totle.time);
    recv_ins_totle.update_flag=1;

    recv_ins_totle.bInitStatusPosition=ins_raw_msg->nInitStatusPosition;
    recv_ins_totle.bInitStatusVel=ins_raw_msg->nInitstatusVel;
    recv_ins_totle.bInitStatusH=ins_raw_msg->nInitstatusH;
    recv_ins_totle.bInitStatusRP=ins_raw_msg->nInitstatusRP;
    recv_ins_totle.iGPSPositionMode=ins_raw_msg->iGPSPositionMode;
    recv_ins_totle.iGPSVelMode=ins_raw_msg->iGPSVelMode;
    recv_ins_totle.iDaulAntennaOrientationMode=ins_raw_msg->iDaulAntennaOrientationMode;
    recv_ins_totle.iSINSMode=ins_raw_msg->iSINSMode;

}

//int Strapdowncal::set_uwb_data()
//{
//    int uwbtimeupdate_flag=0;
//    auto* tmp_driveruwb=ChannelManager::GET_DriverUwb_Adapter()->GetData();
//    if(ChannelManager::GET_DriverUwb_Adapter()->GetValid())
//    {
//        recv_uwb_totle.time=tmp_driveruwb->header().timestamp_sec();
//        recv_uwb_totle.longitude=tmp_driveruwb->uwbpoint().lon();
//        recv_uwb_totle.latitude=tmp_driveruwb->uwbpoint().lat();
//        recv_uwb_totle.nLongitudeQuality=tmp_driveruwb->ncredit();
//        recv_uwb_totle.nLatitudeQuality=tmp_driveruwb->ncredit();
//        recv_uwb_totle.update_flag=tmp_driveruwb->paddinggap();
//        uwbtimeupdate_flag = recv_uwb_totle.update_flag;

//        if( ( fabs(recv_uwb_totle.longitude) > 0.001 ) && ( fabs(recv_uwb_totle.latitude) > 0.001) )
//        {
//            recv_uwb_totle.valid = 1;
//        }
//        else
//        {
//            recv_uwb_totle.valid = 0;
//        }
//    }
//    else
//    {
//        uwbtimeupdate_flag=0;
//    }
//    return uwbtimeupdate_flag;
//}

Strapdowncal::Strapdowncal()
{
    Cbt.setZero( 3, 3 );
    Ctb.setZero( 3, 3 );

    wie_n.setZero( 3, 1 );
    wet_t.setZero( 3, 1 );
    wit_t.setZero( 3, 1 );
    wit_b.setZero( 3, 1 );
    wtb_b.setZero( 3, 1 );

    zt4.setZero( 4, 1 );

    sita.setZero( 4, 4 );
    CbtM.setZero( 3, 3 );
    CtbM.setZero( 3, 3 );
    f_INSt.setZero( 3, 1 );
    acc.setZero( 3, 1 );
    Vel.setZero( 3, 1 );
    Vellast.setZero( 3, 1 );

    cross1.setZero( 3, 1 );
    gMatrix.setZero( 3, 1 );

    eye4.setIdentity( 4, 4 );

    init_flag=0;
}

#if 1
void Strapdowncal::initialization(CoupleOutput *send_sins_last, SensorDataInput *recv_sins_totle, InternalData  *Internal_sins)
{
    send_sins_last->time = recv_sins_totle->time;


    send_sins_last->heading = recv_sins_totle->heading*RAD2DEGREE;
    send_sins_last->pitch   = -recv_sins_totle->fPitch*RAD2DEGREE;
    send_sins_last->roll    = recv_sins_totle->fRoll*RAD2DEGREE;

    send_sins_last->vn = recv_sins_totle->fVelNorth;
    send_sins_last->vw = recv_sins_totle->fVelWest;
    send_sins_last->vu = recv_sins_totle->fVelUp;

    Vel( 0 ) = -send_sins_last->vw;
    Vel( 1 ) = send_sins_last->vn;
    Vel( 2 ) = send_sins_last->vu;

    Vellast( 0 ) = Vel( 0 );
    Vellast( 1 ) = Vel( 1 );
    Vellast( 2 ) = Vel( 2 );


    send_sins_last->latitude = recv_sins_totle->latitude;
    send_sins_last->longitude = recv_sins_totle->longitude;
    send_sins_last->height = recv_sins_totle->height;


    Cbt_cal(send_sins_last->heading, send_sins_last->pitch, send_sins_last->roll);

    Ctb = Cbt.transpose();

    Internal_sins->Rx = Re/(1.0-e*pow(sin(send_sins_last->latitude*DEGREE2RAD),2));
    Internal_sins->Ry = Re/(1.0+2.0*e-3.0*e*pow(sin(send_sins_last->latitude*DEGREE2RAD),2));

    wie_n( 0 ) = 0.0;
    wie_n( 1 ) = W_ie*cos(send_sins_last->latitude*DEGREE2RAD);
    wie_n( 2 ) = W_ie*sin(send_sins_last->latitude*DEGREE2RAD);

    wet_t( 0 )= -Vellast(1)/(Internal_sins->Ry+send_sins_last->height);
    wet_t( 1 )= Vellast(0)/(Internal_sins->Rx+send_sins_last->height);
    wet_t( 2 )= Vellast(0)*tan(send_sins_last->latitude*DEGREE2RAD)/(Internal_sins->Rx+send_sins_last->height);


    wit_t=wie_n+wet_t;
    wit_b=Cbt*wit_t;

    zitai_4(send_sins_last->heading,send_sins_last->pitch,send_sins_last->roll);

}


//void Strapdowncal::initialization_with_uwb(CoupleOutput *send_sins_last, SensorDataInput *recv_sins_totle, SensorDataInput *recv_sins_uwb_totle,InternalData  *Internal_sins)
//{
//    send_sins_last->time = recv_sins_totle->time;


//    send_sins_last->heading = recv_sins_totle->heading*RAD2DEGREE;
//    send_sins_last->pitch   = -recv_sins_totle->fPitch*RAD2DEGREE;
//    send_sins_last->roll    = recv_sins_totle->fRoll*RAD2DEGREE;

//    send_sins_last->vn = recv_sins_totle->fVelNorth;
//    send_sins_last->vw = recv_sins_totle->fVelWest;
//    send_sins_last->vu = recv_sins_totle->fVelUp;

//    Vel( 0 ) = -send_sins_last->vw;
//    Vel( 1 ) = send_sins_last->vn;
//    Vel( 2 ) = send_sins_last->vu;

//    Vellast( 0 ) = Vel( 0 );
//    Vellast( 1 ) = Vel( 1 );
//    Vellast( 2 ) = Vel( 2 );


//    send_sins_last->latitude = recv_sins_uwb_totle->latitude;
//    send_sins_last->longitude = recv_sins_uwb_totle->longitude;
//    send_sins_last->height = recv_sins_totle->height;


//    Cbt_cal(send_sins_last->heading, send_sins_last->pitch, send_sins_last->roll);

//    Ctb = Cbt.transpose();

//    Internal_sins->Rx = Re/(1.0-e*pow(sin(send_sins_last->latitude*DEGREE2RAD),2));
//    Internal_sins->Ry = Re/(1.0+2.0*e-3.0*e*pow(sin(send_sins_last->latitude*DEGREE2RAD),2));

//    wie_n( 0 ) = 0.0;
//    wie_n( 1 ) = W_ie*cos(send_sins_last->latitude*DEGREE2RAD);
//    wie_n( 2 ) = W_ie*sin(send_sins_last->latitude*DEGREE2RAD);

//    wet_t( 0 )= -Vellast(1)/(Internal_sins->Ry+send_sins_last->height);
//    wet_t( 1 )= Vellast(0)/(Internal_sins->Rx+send_sins_last->height);
//    wet_t( 2 )= Vellast(0)*tan(send_sins_last->latitude*DEGREE2RAD)/(Internal_sins->Rx+send_sins_last->height);


//    wit_t=wie_n+wet_t;
//    wit_b=Cbt*wit_t;

//    zitai_4(send_sins_last->heading,send_sins_last->pitch,send_sins_last->roll);

//}




void Strapdowncal::sinsdata_transfer(SensorDataInput *recv_sins_totle,SensorDataInput *recv_sins_cal)
{
    //get sins data & transfer (Angrate,Acc)
    recv_sins_cal->fAngRateX = -recv_sins_totle->fAngRateY;//unit  rad/s
    recv_sins_cal->fAngRateY = recv_sins_totle->fAngRateX;//unit  rad/s
    recv_sins_cal->fAngRateZ = recv_sins_totle->fAngRateZ;//unit  rad/s

    recv_sins_cal->fAccX = -recv_sins_totle->fAccY;
    recv_sins_cal->fAccY = recv_sins_totle->fAccX;
    recv_sins_cal->fAccZ = recv_sins_totle->fAccZ;


}

//依次绕roll pitch yaw 旋转 
void Strapdowncal::Cbt_cal(double x, double y, double z)
{
    double fai = x*DEGREE2RAD;//heading
    double si = y*DEGREE2RAD;//pitch 
    double ga = z*DEGREE2RAD;//roll
    Cbt( 0, 0 ) = cos(ga)*cos(fai)-sin(ga)*sin(si)*sin(fai);
    Cbt( 0, 1 ) = cos(ga)*sin(fai)+sin(ga)*sin(si)*cos(fai);
    Cbt( 0, 2 ) = -sin(ga)*cos(si);
    Cbt( 1, 0 ) = -cos(si)*sin(fai);
    Cbt( 1, 1 ) = cos(si)*cos(fai);
    Cbt( 1, 2 ) = sin(si);
    Cbt( 2, 0 ) = sin(ga)*cos(fai)+cos(ga)*sin(si)*sin(fai);
    Cbt( 2, 1 ) = sin(ga)*sin(fai)-cos(ga)*sin(si)*cos(fai);
    Cbt( 2, 2 ) = cos(ga)*cos(si);

}

void Strapdowncal::zitai_4(double heading,double pitch,double roll)
{
    double x = pitch*DEGREE2RAD;
    double y = roll*DEGREE2RAD;
    double z = heading*DEGREE2RAD;

    zt4( 0 ) = cos(z/2.0)*cos(x/2.0)*cos(y/2.0)-sin(z/2.0)*sin(x/2.0)*sin(y/2.0);
    zt4( 1 ) = cos(z/2.0)*sin(x/2.0)*cos(y/2.0)-sin(z/2.0)*cos(x/2.0)*sin(y/2.0);
    zt4( 2 ) = cos(z/2.0)*cos(x/2.0)*sin(y/2.0)+sin(z/2.0)*sin(x/2.0)*cos(y/2.0);
    zt4( 3 ) = cos(z/2.0)*sin(x/2.0)*sin(y/2.0)+sin(z/2.0)*cos(x/2.0)*cos(y/2.0);


}

void Strapdowncal::sita_cal(double delta_time)
{
    sita( 0, 0 )= 0.0;
    sita( 0, 1 )= -wtb_b( 0 );
    sita( 0, 2 )= -wtb_b( 1 );
    sita( 0, 3 )= -wtb_b( 2 );
    sita( 1, 0 )= wtb_b( 0 );
    sita( 1, 1 )= 0.0;
    sita( 1, 2 )= wtb_b( 2 );
    sita( 1, 3 )= -wtb_b( 1 );
    sita( 2, 0 )= wtb_b( 1 );
    sita( 2, 1 )= -wtb_b( 2 );
    sita( 2, 2 )= 0.0;
    sita( 2, 3 )= wtb_b( 0 );
    sita( 3, 0 )= wtb_b( 2 );
    sita( 3, 1 )= wtb_b( 1 );
    sita( 3, 2 )= -wtb_b( 0 );
    sita( 3, 3 )=  0.0;

    sita=(delta_time)*sita;


}

void Strapdowncal::strapdown_calculating(double& lat,double& lon,double& height,
                                         double& time,double& heading,
                                         const std::shared_ptr<VCU_VEHILCE_INFO> &vcu_raw_msg,
                                         const std::shared_ptr<INERTIAL_Debug> &ins_raw_msg)
{
    set_vcu_data(vcu_raw_msg);
    set_ins_data(ins_raw_msg);
    //set_uwb_data();

    struct SensorDataInput recv_sins_totle = recv_ins_totle;
    //struct SensorDataInput recv_sins_uwb_totle=recv_uwb_totle;
    struct CoupleOutput send_sins = send_sins_totle;
    struct CoupleOutput send_sins_last = send_sins_last_totle;
    struct CoupleOutput send_sins_vcu = send_sins_vcu_totle;
    struct CoupleOutput send_sins_vcu_last = send_sins_vcu_last_totle;
    struct VCU_data        recv_vcu =recv_vcu_totle;

    struct InternalData    Internal_sins = Internal_sins_data;
    struct SensorDataInput recv_sins_cal = recv_sins_cal_data;


    /************************coordinate transfer************************/
    //transfer the Gyro and Accelerator data
    sinsdata_transfer( &recv_sins_totle, &recv_sins_cal);


    /****************************calc count *************************/
    if(recv_ins_totle.iGPSPositionMode==4)
    {
        Internal_sins.count = Internal_sins.count + 1;
        if(Internal_sins.count>100)
        {
            Internal_sins.count=0;
        }
    }


    // Internal_sins.count = Internal_sins.count + 1;

    /***************************strapdown calc**********************/
    /**************init*********************/
    // if( ( 0 == init_flag ) && ( recv_ins_totle.iGPSPositionMode==4 ) )
    //if( ( 0 == init_flag ) &&(recv_ins_totle.iGPSPositionMode==4) &&(recv_ins_totle.iDaulAntennaOrientationMode==4)&&(recv_ins_totle.bInitStatusPosition==1)&&(recv_ins_totle.bInitStatusH==1)&&(recv_ins_totle.bInitStatusRP==1))
    if( ( 0 == init_flag ) &&(recv_ins_totle.iGPSPositionMode>3)&&(recv_ins_totle.iDaulAntennaOrientationMode>3) &&(recv_ins_totle.bInitStatusPosition==1)&&(recv_ins_totle.bInitStatusH==1)&&(recv_ins_totle.bInitStatusRP==1))
    {
        //initial
        double nowtime =recv_sins_totle.time;
        time = nowtime;
        initialization(&send_sins_last, &recv_sins_totle, &Internal_sins);
        send_sins = send_sins_last;

        send_sins_vcu = send_sins;
        send_sins_vcu_last = send_sins;

        init_flag = 1;
        Internal_sins.count=0;
    }
    //else if(( 0 == init_flag ) && ( Internal_sins.count>4 )&&(recv_sins_uwb_totle.longitude>0.01)&&(recv_sins_uwb_totle.latitude>0.01))
    //    else if(( 0 == init_flag ) &&(recv_sins_uwb_totle.longitude>1.0)&&(recv_sins_uwb_totle.latitude>1.0)&&(recv_ins_totle.bInitStatusPosition==1)&&(recv_ins_totle.bInitStatusH==1)&&(recv_ins_totle.bInitStatusRP==1))
    //    {
    //        double nowtime=recv_sins_totle.time;
    //        time = nowtime;
    //        initialization_with_uwb(&send_sins_last, &recv_sins_totle, &recv_sins_uwb_totle,&Internal_sins);
    //        send_sins = send_sins_last;

    //        send_sins_vcu = send_sins;
    //        send_sins_vcu_last = send_sins;

    //        init_flag = 1;
    //        Internal_sins.count=0;
    //    }
    /***************************************************************/

    if( 1 == init_flag )
    {
        /****************************get nowtime****************************/
        double nowtime =recv_sins_totle.time;
        time = nowtime;
        //std::cout<<"nowtime:"<<time<<endl;
        send_sins.time = nowtime;
        double delta_time = send_sins.time-send_sins_last.time;
        //std::cout<<"send_sins.time:"<<send_sins.time<<"send_sins_last.time:"<<send_sins_last.time<<"delt_time:"<<delta_time<<std::endl;
#if 0
        if( (fabs( send_sins.time-send_sins_last.time ) > 0.00001)&&(fabs( send_sins.time-send_sins_last.time ) < 0.03) )
        {
            delta_time = send_sins.time-send_sins_last.time; ;//sec
        }
        else if(fabs( send_sins.time-send_sins_last.time ) >= 0.03)
        {
            delta_time =0.02;
        }
        else
        {
            delta_time = 0.00000000000001;//error
        }
#else
        if( (fabs( send_sins.time-send_sins_last.time ) > 0.00001)&&(fabs( send_sins.time-send_sins_last.time ) < 0.5) )
        {
            delta_time = send_sins.time-send_sins_last.time; ;//sec
        }
        else
        {
            delta_time = 0.00000000000001;//error
        }

        //  delta_time=0.02;
#endif

        //std::cout<<"delta_time:"<<delta_time<<endl;

        wtb_b( 0 ) = recv_sins_cal.fAngRateX;
        wtb_b( 1 ) = recv_sins_cal.fAngRateY;
        wtb_b( 2 ) = recv_sins_cal.fAngRateZ;
        wtb_b = wtb_b-wit_b;

        double sita02 = (wtb_b( 0 )*delta_time)*(wtb_b( 0 )*delta_time)+(wtb_b( 1 )*delta_time)*(wtb_b( 1 )*delta_time)+(wtb_b( 2 )*delta_time)*(wtb_b( 2 )*delta_time);
        double sita0 =sqrt(sita02);
        sita_cal(delta_time);
        zt4 =(cos(sita0/2.0)*eye4 + (sin(sita0/2.0)/sita0)*sita)*zt4;
        double zt4_norm = sqrt( pow(zt4(0),2)+pow(zt4(1),2)+pow(zt4(2),2)+pow(zt4(3),2));
        zt4 = zt4/zt4_norm;

        CbtM( 0, 0 )= pow(zt4(0),2)+pow(zt4(1),2)-pow(zt4(2),2)-pow(zt4(3),2);
        CbtM( 0, 1 )= 2.0*(zt4(1)*zt4(2)+zt4(0)*zt4(3));
        CbtM( 0, 2 )= 2.0*(zt4(1)*zt4(3)-zt4(0)*zt4(2));
        CbtM( 1, 0 )= 2.0*(zt4(1)*zt4(2)-zt4(0)*zt4(3));
        CbtM( 1, 1 )= pow(zt4(0),2)-pow(zt4(1),2)+pow(zt4(2),2)-pow(zt4(3),2);
        CbtM( 1, 2 )= 2.0*(zt4(2)*zt4(3)+zt4(0)*zt4(1));
        CbtM( 2, 0 )= 2.0*(zt4(1)*zt4(3)+zt4(0)*zt4(2));
        CbtM( 2, 1 )= 2.0*(zt4(2)*zt4(3)-zt4(0)*zt4(1));
        CbtM( 2, 2 )= pow(zt4(0),2)-pow(zt4(1),2)-pow(zt4(2),2)+pow(zt4(3),2);
        CtbM =CbtM.transpose();

        f_INSt(0)=CtbM(0,0)*recv_sins_cal.fAccX+CtbM(0,1)*recv_sins_cal.fAccY+CtbM(0,2)*recv_sins_cal.fAccZ ;
        f_INSt(1)=CtbM(1,0)*recv_sins_cal.fAccX+CtbM(1,1)*recv_sins_cal.fAccY+CtbM(1,2)*recv_sins_cal.fAccZ ;
        f_INSt(2)=CtbM(2,0)*recv_sins_cal.fAccX+CtbM(2,1)*recv_sins_cal.fAccY+CtbM(2,2)*recv_sins_cal.fAccZ ;


        double g=g0*(1.0+gk1*pow(sin(send_sins_last.latitude*DEGREE2RAD),2))*(1.0-2.0*send_sins_last.height/Re)/sqrt(1.0-gk2*pow(sin(send_sins_last.latitude*DEGREE2RAD),2));

        cross1( 0 )= (2.0*wie_n(1)+wet_t(1))*Vellast(2)-(2.0*wie_n(2)+wet_t(2))*Vellast(1);
        cross1( 1 )= (2.0*wie_n(2)+wet_t(2))*Vellast(0)-(2.0*wie_n(0)+wet_t(0))*Vellast(2);
        cross1( 2 )= (2.0*wie_n(0)+wet_t(0))*Vellast(1)-(2.0*wie_n(1)+wet_t(1))*Vellast(0);
        gMatrix( 0 )=0.0;
        gMatrix( 1 )=0.0;
        gMatrix( 2 )=-g;

        acc = f_INSt - cross1 + gMatrix;

        Vel = Vellast + (delta_time)*acc;
        send_sins.vw = -Vel(0);
        send_sins.vn = Vel(1);
        send_sins.vu = Vel(2);

        send_sins.latitude = send_sins_last.latitude + (delta_time)*(Vellast(1)+Vel(1))*RAD2DEGREE/(2.0*(Internal_sins.Ry+send_sins_last.height));
        send_sins.longitude = send_sins_last.longitude + (delta_time)*(Vellast(0)+Vel(0))*RAD2DEGREE/(2.0*(Internal_sins.Rx+send_sins_last.height)*cos(send_sins_last.latitude*DEGREE2RAD));
        send_sins.height = send_sins_last.height + (delta_time)*(Vellast(2)+Vel(2))/2.0;

        double Rx_temp = Internal_sins.Rx;
        double Ry_temp = Internal_sins.Ry;

        Internal_sins.Rx = Re/(1.0-e*pow(sin(send_sins.latitude*DEGREE2RAD),2));
        Internal_sins.Ry = Re/(1.0+2.0*e-3.0*e*pow(sin(send_sins.latitude*DEGREE2RAD),2));

        wie_n( 0 )= 0.0;
        wie_n( 1 )= W_ie*cos(send_sins.latitude*DEGREE2RAD);
        wie_n( 2 )= W_ie*sin(send_sins.latitude*DEGREE2RAD);
        wet_t( 0 )= -Vel(1)/(Internal_sins.Ry+send_sins.height);
        wet_t( 1 )= Vel(0)/(Internal_sins.Rx+send_sins.height);
        wet_t( 2 )= Vel(0)*tan(send_sins.latitude*DEGREE2RAD)/(Internal_sins.Rx+send_sins.height);

        wit_t=wie_n+wet_t;
        wit_b=CbtM*wit_t;

        /**********************attitude calculate*******************/
        //pitch calc
        send_sins.pitch = asin(CbtM(1,2))*RAD2DEGREE;

        //roll calc
        if (fabs(CbtM(2,2))<eps)
        {
            if(CbtM(0,2)>0.0)
            {
                send_sins.roll=-90.0;
            }
            else
            {
                send_sins.roll=90.0;
            }
        }
        else
        {
            send_sins.roll=atan(-CbtM(0,2)/CbtM(2,2))*RAD2DEGREE;
            if(CbtM(2,2)<0.0)
            {
                if(CbtM(0,2)>0.0)
                {
                    send_sins.roll=send_sins.roll-180.0;
                }
                else
                {
                    send_sins.roll=send_sins.roll+180.0;
                }
            }
        }

        //heading calc
        if (fabs(CbtM(1,1))>eps)
        {
            send_sins.heading =atan(-CbtM(1,0)/CbtM(1,1))*RAD2DEGREE;
            if(CbtM(1,1)>0.0)
            {
                if(CbtM(1,0)>0.0)
                {
                    send_sins.heading=send_sins.heading+360.0;
                }
            }
            else
            {
                send_sins.heading=send_sins.heading+180.0;
            }
        }
        else
        {
            if(CbtM(1,0)<0.0)
            {
                send_sins.heading=90.0;
            }
            else
            {
                send_sins.heading=270.0;
            }
        }

        if((recv_ins_totle.iGPSPositionMode==4)&&(recv_ins_totle.iDaulAntennaOrientationMode==4))
        {
            send_sins.heading=recv_ins_totle.heading*RAD2DEGREE;
            send_sins.pitch=-recv_ins_totle.fPitch*RAD2DEGREE;
            send_sins.roll=recv_ins_totle.fRoll*RAD2DEGREE;
        }
        zitai_4(send_sins.heading,send_sins.pitch,send_sins.roll);



        /**********************Dr**************************/

        double speed = fabs((recv_vcu.fRearLeftWheelSpeed+recv_vcu.fRearRightWheelSpeed)/2.0);
        //double speed = 1.08*fabs((recv_vcu.fRearLeftWheelSpeed+recv_vcu.fRearRightWheelSpeed)/2.0);

        if((recv_vcu.fRearLeftWheelSpeed<0.02)||(recv_vcu.fRearRightWheelSpeed<0.02))
        {
            speed=0.0;
        }
        double vcu_speed=recv_vcu.fSpeed;

        double ins_speed=sqrt(pow(recv_ins_totle.fVelNorth,2)+pow(recv_ins_totle.fVelWest,2)+pow(recv_ins_totle.fVelUp,2));
        //double speed=ins_speed;
        //std::cout<<"delta_speed:"<<ins_speed-speed<<endl;
        //speed=speed*1.009;
        //speed=ins_speed;
        //speed=vcu_speed;
#if 1
        double speed_east= -speed*cos(send_sins.pitch*DEGREE2RAD)*sin(send_sins.heading*DEGREE2RAD);
        double speed_north= speed*cos(send_sins.pitch*DEGREE2RAD)*cos(send_sins.heading*DEGREE2RAD);
        double speed_up= speed*sin(send_sins.pitch*DEGREE2RAD);
        //        double speed_east= -speed*sin(send_sins.heading*DEGREE2RAD);
        //        double speed_north= speed*cos(send_sins.heading*DEGREE2RAD);
        //        double speed_up= speed*sin(send_sins.pitch*DEGREE2RAD);
#else

        //        double speed_east=-send_sins.vw;
        //        double speed_north=send_sins.vn;
        //        double speed_up=send_sins.vu;
//        double speed_east=-recv_ins_totle.fVelWest;
//        double speed_north=recv_ins_totle.fVelNorth;
//        double speed_up=recv_ins_totle.fVelUp;

#endif

        if (recv_vcu.nShifterPosition==7)
        {
            speed_east = (-1.0) *speed_east;
            speed_north = (-1.0) *speed_north;
            speed_up = (-1.0) *speed_up;
        }
        else if(recv_vcu.nShifterPosition==8)
        {
            speed_east = 0.0;
            speed_north = 0.0;
            speed_up = 0.0;
        }


        send_sins_vcu.latitude = send_sins_vcu_last.latitude + delta_time*speed_north/(Ry_temp + send_sins_vcu_last.height)*RAD2DEGREE;
        send_sins_vcu.longitude = send_sins_vcu_last.longitude + delta_time*speed_east/((Rx_temp +send_sins_vcu_last.height)*cos(send_sins_vcu_last.latitude*DEGREE2RAD))*RAD2DEGREE;
        send_sins_vcu.height = send_sins_vcu_last.height + delta_time*speed_up;

#if 1
        lat = send_sins_vcu.latitude;
        lon = send_sins_vcu.longitude;
        height = send_sins_vcu.height;
        heading = send_sins.heading*DEGREE2RAD;
#else
        lat = recv_ins_totle.latitude;
        lon = recv_ins_totle.longitude;
        height = recv_ins_totle.height;
        heading = recv_ins_totle.heading;
#endif
#if 0
        std::cout<<"Wheelspeed: " <<recv_vcu.fFrontLeftWheelSpeed<<" "<<recv_vcu.fFrontRightWheelSpeed<<" "<<recv_vcu.fRearLeftWheelSpeed<<" "<<recv_vcu.fRearRightWheelSpeed<<endl;
        std::cout<<"vcuspeed:"<<recv_vcu.fSpeed<<endl;
        std::cout<<"fabspeed:"<<speed<<endl;
        std::cout<<"insspeed:"<<ins_speed<<endl;
//        std::cout<<"heading:"<<send_sins.heading<<endl;
//        std::cout<<"pitch:"<< send_sins.pitch<<endl;
//        std::cout<<"roll:"<< send_sins.roll<<endl;
//        std::cout<<"lon:"<<setiosflags( ios::fixed ) << setprecision( 9 )<<lon<<endl;
//        std::cout<<"lat:"<<setiosflags( ios::fixed ) << setprecision( 9 )<<lat<<endl;
#endif


        /**************************************************/

    }

    /**********************cout Dr results*************************/
#if 0
    double delt_latitude2=(send_sins_vcu_totle.latitude-recv_sins_totle.latitude)*DEGREE2RAD*(Internal_sins.Ry);
    double delt_longitude2=(send_sins_vcu_totle.longitude-recv_sins_totle.longitude)*DEGREE2RAD*(Internal_sins.Rx);
    double delt_distance2=sqrt(pow(delt_latitude2,2)+pow(delt_longitude2,2));

    std::cout<<setiosflags( ios::fixed ) << setprecision( 9 )<<send_sins_vcu_totle.latitude<<"  "<<recv_ins_totle.latitude<<"  "<<delt_latitude2<<endl;
    std::cout<<setiosflags( ios::fixed ) << setprecision( 9 )<<send_sins_vcu_totle.longitude<<"  "<<recv_ins_totle.longitude<<"  "<<delt_longitude2<<endl;
    std::cout<<setiosflags( ios::fixed ) << delt_distance2<<endl;
    std::cout<<setiosflags( ios::fixed ) << setprecision( 9 )<<send_sins.heading<<" "<<recv_ins_totle.heading*RAD2DEGREE<<" "<<send_sins.heading-recv_ins_totle.heading*RAD2DEGREE<<endl;
    std::cout<<setiosflags( ios::fixed ) << setprecision( 9 )<<-send_sins.pitch<<" "<<recv_ins_totle.fPitch*RAD2DEGREE<<" "<<-send_sins.pitch-recv_ins_totle.fPitch*RAD2DEGREE<<endl;
    std::cout<<setiosflags( ios::fixed ) << setprecision( 9 )<<send_sins.roll<<" "<<recv_ins_totle.fRoll*RAD2DEGREE<<" "<<send_sins.roll-recv_ins_totle.fRoll*RAD2DEGREE<<endl;
#endif



    /*****************Backup data*********************/
    //Bak position
    send_sins_last.latitude=send_sins.latitude;
    send_sins_last.longitude=send_sins.longitude;
    send_sins_last.height=send_sins.height;

    //Bak velocity
    Vellast(0)=Vel(0);
    Vellast(1)=Vel(1);
    Vellast(2)=Vel(2);

    //Bak attitude
    send_sins_last.heading=send_sins.heading;
    send_sins_last.pitch=send_sins.pitch;
    send_sins_last.roll=send_sins.roll;
    send_sins_last.time=send_sins.time;

    //Bak Dr position
    send_sins_vcu_last.latitude = send_sins_vcu.latitude;
    send_sins_vcu_last.longitude = send_sins_vcu.longitude;
    send_sins_vcu_last.height = send_sins_vcu.height;


    Internal_sins_data= Internal_sins;
    recv_sins_cal_data = recv_sins_cal;
    //recv_ins_totle = recv_sins_totle;

    send_sins_totle = send_sins;
    send_sins_last_totle = send_sins_last;

    send_sins_vcu_totle = send_sins_vcu;
    send_sins_vcu_last_totle = send_sins_vcu_last;

    
    
    

}
#endif

}  // namespace dr
}  // namespace atd
