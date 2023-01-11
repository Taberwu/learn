#include "modules/sp_dr/include/triggers/vcu_dr.h"
#include "platform/WorldModel/include/Concept/geo/linear_interpolation.h"
#include "platform/WorldModel/include/Concept/geo/angle.h"
namespace atd {
namespace sp_dr {

using atd::worldmodel::lerp;
using atd::worldmodel::Angle32;

VCUdr::VCUdr(){}

float VCUdr::MoveRadiusCalibration(float f_steering_angle)
{
    //initialize the index for interpolation
    int n_index = 0;

    //avoid out_of_range problem
    if(f_steering_angle<fStrgWheelAngleList[0])
    {
        f_steering_angle = fStrgWheelAngleList[0];
        //<<"The actual steering exceeds the lower limit of the steering wheel angle!";
    }
    else if(f_steering_angle>fStrgWheelAngleList[STRGWHEELCALIBRATIONSIZE-1])
    {
        f_steering_angle = fStrgWheelAngleList[STRGWHEELCALIBRATIONSIZE-1];
        fprintf(stderr,"The actual steering exceeds the upper limit of the steering wheel angle!");
    }

    //search the position in the steering wheel angle list
    for(int i = 0;i<STRGWHEELCALIBRATIONSIZE-2;i++)
    {
        if (fStrgWheelAngleList[i]<f_steering_angle && fStrgWheelAngleList[i+1]>=f_steering_angle)
        {
            n_index = i;
        }
    }

    //interpolation
    float cur_radius=fMoveRadiusList[n_index];
    float next_radius=fMoveRadiusList[n_index+1];
    float cur_angle=fStrgWheelAngleList[n_index];
    float next_angle=fStrgWheelAngleList[n_index+1];
    float f_move_radius =lerp(cur_radius, cur_angle, next_radius, next_angle,f_steering_angle);
    return fabs(f_move_radius);
}

void VCUdr::dr_process(const float f_ave_wheel_speed, const float steerangle, const int gear,const float time_diff,Arrow2d &result)
{
    double delta_x = 0;
    double delta_y = 0;

    //move_radius(degree input)
    double move_radius=Radiusfix*MoveRadiusCalibration(Angle32::rad2deg(steerangle));
    //delta_yaw
    double delta_yaw = atan(time_diff *f_ave_wheel_speed /move_radius) ;
    //AERROR<<"diff_time:"<<time_diff<<"\tmove_radius:"<<move_radius<<"\t speed:"<<f_ave_wheel_speed;
    //consider the back off
    double gap=time_diff *f_ave_wheel_speed;

    if (-1 == gear )
    {
        //R GEAR
        if(steerangle > 0){
            delta_yaw = -delta_yaw;
        }

        if (steerangle > 0){
            delta_x =  move_radius * sin(delta_yaw);
            delta_y =  move_radius * (1.0 - cos(delta_yaw));
        }
        else
        {
            delta_x = -move_radius * sin(delta_yaw);
            delta_y = -move_radius * (1.0 - cos(delta_yaw));
        }
    }
    else if (1 == gear )
    {
        //D GEAR
        if(steerangle < 0){
            delta_yaw = -delta_yaw;
        }

        if (steerangle > 0){
            delta_x =  move_radius * sin(delta_yaw);
            delta_y =  move_radius * (1.0 - cos(delta_yaw));
        }else{
            delta_x = -move_radius * sin(delta_yaw);
            delta_y = -move_radius * (1.0 - cos(delta_yaw));
        }
    }
    else
    {
        delta_x = 0;
        delta_y = 0;
        delta_yaw = 0;
    }

    //output
    result.set_center_x(delta_x);
    result.set_center_y(delta_y);
    result.set_heading(delta_yaw);
}


}
}
