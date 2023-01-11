#ifndef ASSIST_H
#define ASSIST_H

#include <math.h>

namespace atd {
namespace sp_dr {

const double Pi = 3.141592653589793;
const double DEGREE2RAD = 0.017453292519943; //0.017453293//Pi/180.0
const double RAD2DEGREE = 57.295779513082323; //57.295779513//180.0/Pi
const float MAX_ANGLERATE = 30; //20dps
const double rte_timetosec_factor = 0.000000001;//ns->s
struct CRD_LLA
{
    double longitude;/*degree*/
    double latitude;/*degree*/
    float  height;/*!<height from the ground*/
    float  heading;/*ni,degree,0~360*/
    CRD_LLA()
    {
    longitude = 0.0f;
    latitude = 0.0f;
    height = 0.0f;
    heading = 0.0f;
    }
};

struct CRD_Body
{
    float  bx;/*bx*/
    float  by;/*by*/
    float  dheight;/*dheight*/
    float  dheading;/*ni,degree,0~360*/

};

struct CRD_Gauss
{
    double  x;/*x-north*/
    double  y;/*y-east*/
    float  heading;/*heading-shun,degree*/
    float  height;/*height*/
};

struct VCU_data
{
    double  time;
    float  fSteeringAngle;

    float  fSpeed;

    float  fYawRate;

    float  fFrontLeftWheelSpeed;

    float  fFrontRightWheelSpeed;

    float  fRearLeftWheelSpeed;

    float  fRearRightWheelSpeed;

    int  nShifterPosition;

    int    nLeftDirectionLamp;

    int    nRightDirectionLamp;

    int    nMainBeamLamp;

    int    nDippedBeamLamp;

    int    nWiperState;

    float   fLateralAccel;

    float   fLongituAccel;

    int    nLeftDrivenWheelPulseCounters;

    int    nRightDrivenWheelPulseCounters;

    int    nLeftNonDrivenWheelPulseCounters;

    int   nRightNonDrivenWheelPulseCounters;

    int    nDriveMode;

    bool   update_flag;
};

struct CoupleOutput
{
    double   time;//s
    double  longitude;//degree
    double  latitude;//degree
    double  height;//m

    double   vw;
    double   vn;
    double   vu;
    double   speed;

    double   heading;// ni, rad [0,2*PI]
    double   pitch;
    double   roll;

    double  delta_bx;
    double  delta_by;
    double  delta_bz;

    double   d_gyrox;
    double   d_gyroy;
    double   d_gyroz;

    double   d_accx;//m/s/s
    double   d_accy;
    double   d_accz;

    double   d_speed;
    double  d_heading;//rad/s
    bool   valid;


};


struct SensorDataInput
{

    double   time;//s
    double  longitude;
    double  latitude;
    double  height;
    double   heading;
    int   heading_valid;
    double   bx;
    double  by;
    double   dtheta;//rad,ni
    int  valid;
    double  rlongitude;
    double  rlatitude;
    double  rheading;
    double  rbx;
    double  rby;
    double  rv;
    double  rdheading;//degree
    double   fVelNorth;

    double  fVelWest;

    double   fVelUp;

    double   fPitch;

    double   fRoll;
    double fAccX;
    double fAccY;
    double fAccZ;
    double   fAngRateX;
    double   fAngRateY;
    double  fAngRateZ;
    int     bInitStatusPosition;

    int     bInitStatusVel;

    int     bInitStatusRP;

    int    bInitStatusH;

    double      dGPSTime;

    double      stdstd[9];

    int    iGPSPositionMode;

    int    iGPSVelMode;

    int    iDaulAntennaOrientationMode;

    int    iSINSMode;
    double     undertime_totle;
    double     nonfixedtime_totle;
    int      under_flag ;// 0-ground 1-under ground


    double fixedtime_totle;

    bool   update_flag;

    int iParkingStatus;

    //ofilm
    int nLatitudeQuality;
    int nLongitudeQuality;
    int nHeadingQuality;
    float fLatitudeVariance;
    float fLongitudeVariance;
    float fHeadingVariance;

};
struct stdiffpos
{
    double time;//s
    double update_time;//s
    double difftime;
    double longitude;
    double latitude;
    float heading;//ni,rad
    double diff_longitude;
    double diff_latitude;
    double diff_height;
    double diff_north;
    double diff_east;
    int update_flag;
    double rlongitude;
    double rlatitude;
    int choose_flag;
    int new_flag ;
    int init_flag;

};

struct InternalData
{

    int count;
    double Rx;
    double Ry;
    double undertime_start;
    double undertime_end ;
    double undertime_totle;
    //non fixed
    //uint8_t nonfixed_flag ;
    double nonfixedtime_start ;
    double nonfixedtime_end ;
    double nonfixedtime_totle ;
    double fixedtime_start ;
    double fixedtime_end ;
    double fixedtime_totle ;

    int couple_flag ;
    int out_type ;
    //float64 dt ;
    double control_test_start_time ;
};
void LLA2Gauss( const CRD_LLA LLA, CRD_Gauss *stGauss );
void Gauss2LLA( const CRD_Gauss stGauss, CRD_LLA *stlla );
void LLA2B( const CRD_LLA LLA0,const CRD_LLA LLA1, CRD_Body *stbody );
void Body2LLA( const CRD_Body stbody, const CRD_LLA stllalast, CRD_LLA *stllanew );
void Gauss2B( const CRD_Gauss stGauss0, const CRD_Gauss stGauss1, CRD_Body *stbody );
void Body2Gauss( const CRD_Body stbody, const CRD_Gauss stGauss0, CRD_Gauss *stGauss1 );
void test_couple();


}  // namespace localization
}  // namespace atd
#endif
