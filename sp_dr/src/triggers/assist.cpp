#include "modules/sp_dr/include/triggers/assist.h"

namespace atd {
namespace sp_dr {


void test_couple()
{
 int i = 0;
     struct CRD_Body stbody;
    stbody.bx = 0.01231;
    stbody.by = -0.01116;
    stbody.dheading = -0.00454 * RAD2DEGREE;

    struct CRD_LLA stllalast;
    stllalast.longitude = 121.1862031;
    stllalast.latitude = 31.2813866;
    stllalast.heading = 4.388 * RAD2DEGREE;
    stllalast.height = 0;

    struct CRD_LLA stllanew;
    Body2LLA( stbody, stllalast, &stllanew );
 return ;
 }
void LLA2Gauss( const CRD_LLA LLA, CRD_Gauss *stGauss )
{
    int ProjNo=0; int ZoneWide; ////带宽
    double longitude1,latitude1, longitude0,latitude0, X0,Y0, xval,yval;
    double a,f, e2,ee, NN, T,C,A, M;
    ZoneWide = 3; ////6度带宽
    //a=6378245.0; f=1.0/298.3; //54年北京坐标系参数
    a = 6378137;
    f = 1 / 298.257223563;
    ////a=6378140.0; f=1/298.257; //80年西安坐标系参数


    double longitude = LLA.longitude;
    double latitude = LLA.latitude;

    ProjNo = (int)(longitude / ZoneWide) ;
    longitude0 = ProjNo * ZoneWide + ZoneWide / 2;
    longitude0 = longitude0 * DEGREE2RAD ;
    latitude0=0;
    longitude1 = longitude * DEGREE2RAD ; //经度转换为弧度
    latitude1 = latitude * DEGREE2RAD ; //纬度转换为弧度
    e2=2*f-f*f;
    ee=e2*(1.0-e2);
    NN=a/sqrt(1.0-e2*sin(latitude1)*sin(latitude1));
    T=tan(latitude1)*tan(latitude1);
    C=ee*cos(latitude1)*cos(latitude1);
    A=(longitude1-longitude0)*cos(latitude1);
    M=a*((1-e2/4-3*e2*e2/64-5*e2*e2*e2/256)*latitude1-(3*e2/8+3*e2*e2/32+45*e2*e2*e2/1024)*sin(2*latitude1)+(15*e2*e2/256+45*e2*e2*e2/1024)*sin(4*latitude1)-(35*e2*e2*e2/3072)*sin(6*latitude1));
    xval = NN*(A+(1-T+C)*A*A*A/6+(5-18*T+T*T+72*C-58*ee)*A*A*A*A*A/120);
    yval = M+NN*tan(latitude1)*(A*A/2+(5-T+9*C+4*C*C)*A*A*A*A/24+(61-58*T+T*T+600*C-330*ee)*A*A*A*A*A*A/720);
    X0 = 1000000L*(ProjNo+1)+500000L;
    Y0 = 0;
    xval = xval+X0;
    yval = yval+Y0;


    stGauss->x = yval;
    stGauss->y = xval;
    stGauss->heading = -LLA.heading ;// shun,degree
    stGauss->height = LLA.height;
}
void Gauss2LLA( const CRD_Gauss stGauss, CRD_LLA *stlla )
{
    int ProjNo;
    int ZoneWide;
    double longitude1, latitude1, longitude0, latitude0, X0, Y0, xval, yval;
    double e1, e2, f, a, ee, NN, T, C, M, D, R, u, fai, iPI;
    iPI = DEGREE2RAD;

    a = 6378137;
    f = 1 / 298.257223563;

    ZoneWide = 3; // 3 degree width


    double X = stGauss.y;
    double Y = stGauss.x;
    double altitude = stGauss.height;
    double GPSangle = stGauss.heading;//shun,degree

    ProjNo = (int) (X / 1000000L); //find zone in earth
    longitude0 = (ProjNo - 1) * ZoneWide + ZoneWide / 2;
    longitude0 = longitude0 * iPI; //center longitude
    X0 = ProjNo * 1000000L + 500000L;
    Y0 = 0;
    xval = X - X0;
    yval = Y - Y0; //带内大地坐标
    e2 = 2 * f - f * f;
    e1 = (1.0 - sqrt(1 - e2)) / (1.0 + sqrt(1 - e2));
    ee = e2 / (1 - e2);
    M = yval;
    u = M / (a * (1 - e2 / 4 - 3 * e2 * e2 / 64 - 5 * e2 * e2 * e2 / 256));
    fai = u + (3 * e1 / 2 - 27 * e1 * e1 * e1 / 32) * sin(2 * u) +
          (21 * e1 * e1 / 16 - 55 * e1 * e1 * e1 * e1 / 32) * sin(
                  4 * u)
          + (151 * e1 * e1 * e1 / 96) * sin(6 * u) + (1097 * e1 * e1 * e1 * e1 / 512) * sin(8 * u);
    C = ee * cos(fai) * cos(fai);
    T = tan(fai) * tan(fai);
    NN = a / sqrt(1.0 - e2 * sin(fai) * sin(fai));
    R = a * (1 - e2) / sqrt((1 - e2 * sin(fai) * sin(fai)) * (1 - e2 * sin(fai) * sin(fai)) * (1 - e2 * sin
            (fai) * sin(fai)));
    D = xval / NN;
    //计算经度(Longitude) 纬度(Latitude)
    longitude1 = longitude0 +
                 (D - (1 + 2 * T + C) * D * D * D / 6 + (5 - 2 * C + 28 * T - 3 * C * C + 8 * ee + 24 * T * T) * D
                                                        * D * D * D * D / 120) / cos(fai);
    latitude1 = fai - (NN * tan(fai) / R) * (D * D / 2 - (5 + 3 * T + 10 * C - 4 * C * C - 9 * ee) * D * D * D * D / 24
                                             + (61 + 90 * T + 298 * C + 45 * T * T - 256 * ee - 3 * C * C) * D * D * D *
                                               D * D * D / 720);
    //转换为度 DD


    stlla->longitude = longitude1 * RAD2DEGREE;
    stlla->latitude = latitude1 * RAD2DEGREE;
    stlla->heading = -stGauss.heading;// ni,degree
    stlla->height = stGauss.height;

}
void Gauss2B( const CRD_Gauss stGauss0, const CRD_Gauss stGauss1, CRD_Body *stbody )
{

    double originx = stGauss0.x;
    double originy = -stGauss0.y;
    double pointx = stGauss1.x;
    double pointy = -stGauss1.y;

    double angle = stGauss0.heading * DEGREE2RAD;

    stbody->bx = cos(angle) * (pointx - originx) - sin(angle) * (pointy - originy);
    stbody->by = sin(angle) * (pointx - originx) + cos(angle) * (pointy - originy);

    stbody->dheight = stGauss1.height - stGauss0.height;
    float theta;
    theta = -stGauss1.heading + stGauss0.heading ;

    if( theta > 180.0 )
    {
        theta = theta - 360.0;
    }
    else if( theta < -180.0 )
    {
        theta = theta + 360;
    }


    stbody->dheading = theta; //ni,degree
}
void LLA2B( const CRD_LLA LLA0,const CRD_LLA LLA1, CRD_Body *stbody )
{
    CRD_Gauss stGauss0;
    CRD_Gauss stGauss1;
    LLA2Gauss( LLA0, &stGauss0 );
    LLA2Gauss( LLA1, &stGauss1 );
    Gauss2B( stGauss0, stGauss1, stbody );



}
void Body2Gauss( const CRD_Body stbody, const CRD_Gauss stGauss0, CRD_Gauss *stGauss1 )
{
    double pointx = stbody.bx;
    double pointy = stbody.by;

    double angle = stGauss0.heading * DEGREE2RAD;//shun,degree

    stGauss1->x = cos(angle) * (pointx) + sin(angle) * (pointy) + stGauss0.x;
    stGauss1->y = sin(angle) * (pointx) - cos(angle) * (pointy) + stGauss0.y;
    stGauss1->height = stbody.dheight + stGauss0.height;
    stGauss1->heading = stGauss0.heading - stbody.dheading;
}

void Body2LLA( const CRD_Body stbody, const CRD_LLA stllalast, CRD_LLA *stllanew )
{
    CRD_Gauss stGausslast;
    CRD_Gauss stGaussnew;
    LLA2Gauss( stllalast, &stGausslast );
    Body2Gauss( stbody, stGausslast, &stGaussnew );
    Gauss2LLA( stGaussnew, stllanew );


}
}  // namespace localization
}  // namespace atd
