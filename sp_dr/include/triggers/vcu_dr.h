#ifndef MODULES_SP_DR_VCU_DR_H_
#define MODULES_SP_DR_VCU_DR_H_

#include "platform/WorldModel/include/Concept/geo/arrow2d.h"

namespace atd {
namespace sp_dr {

using atd::worldmodel::Arrow2d;

class VCUdr{
public:
    VCUdr();
    virtual ~VCUdr()=default;

    float MoveRadiusCalibration(float f_steering_angle);

    void dr_process(const float f_ave_wheel_speed, const float steerangle, const int gear,const float time_diff,Arrow2d &result);

private:
//    float WHEEL_BASE=2.65;
//    float STEERING_RATIO=18.38;

    int STRGWHEELCALIBRATIONSIZE=17;
    float Radiusfix=1.5;
    float fStrgWheelAngleList[17] ={-500,-300,-150,-80,-50,-30,-20,-10,0,10,20,30,50,80,150,300,500};
    float fMoveRadiusList[17] ={4,5,10,20,40,70,8000,20000,100000,20000,8000,70,40,20,10,5,4};

};

}
}


#endif //MODULES_SP_DR_VCU_DR_H_
