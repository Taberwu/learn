/****************************************************************************************
 * @FilePath: caculateFH.hpp
 * @Author: Taber.wu
 * @Date: 2024-02-26 11:02:26
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-02-26 13:40:37
 * @Copyright: 2024 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef _FHCACULATE_HPP__
#define _FHCACULATE_HPP__
#include <Eigen/Eigen>
#include <Eigen/Core>
#include <Eigen/src/Geometry/Quaternion.h>
#include <string>
#include <iostream>
#include <opencv2/core.hpp>

namespace caculateFH{
    Eigen::Matrix3d computeH21(const std::vector<cv::Point2f> &vP1, const std::vector<cv::Point2f> &vP2)
    {
        const int N = vP1.size();

        Eigen::MatrixXd A(2*N, 9);

        for(int i=0; i<N; i++)
        {
            const double u1 = vP1[i].x;
            const double v1 = vP1[i].y;
            const double u2 = vP2[i].x;
            const double v2 = vP2[i].y;

            A(2*i,0) = 0.0;
            A(2*i,1) = 0.0;
            A(2*i,2) = 0.0;
            A(2*i,3) = -u1;
            A(2*i,4) = -v1;
            A(2*i,5) = -1.;
            A(2*i,6) = v2*u1;
            A(2*i,7) = v2*v1;
            A(2*i,8) = v2;

            A(2*i+1,0) = u1;
            A(2*i+1,1) = v1;
            A(2*i+1,2) = 1.;
            A(2*i+1,3) = 0.0;
            A(2*i+1,4) = 0.0;
            A(2*i+1,5) = 0.0;
            A(2*i+1,6) = -u2*u1;
            A(2*i+1,7) = -u2*v1;
            A(2*i+1,8) = -u2;

        }

        Eigen::JacobiSVD<Eigen::MatrixXd> svd(A, Eigen::ComputeFullV);

        Eigen::Matrix<double,3,3,Eigen::RowMajor> H(svd.matrixV().col(8).data());

        return H;
    }


     Eigen::Matrix3d computeF21(const std::vector<cv::Point2f> &vP1,const std::vector<cv::Point2f> &vP2)
    {
        const int N = vP1.size();

        Eigen::MatrixXd A(N, 9);

        for(int i=0; i<N; i++)
        {
            const double u1 = vP1[i].x;
            const double v1 = vP1[i].y;
            const double u2 = vP2[i].x;
            const double v2 = vP2[i].y;

            A(i,0) = u2*u1;
            A(i,1) = u2*v1;
            A(i,2) = u2;
            A(i,3) = v2*u1;
            A(i,4) = v2*v1;
            A(i,5) = v2;
            A(i,6) = u1;
            A(i,7) = v1;
            A(i,8) = 1.;
        }

        Eigen::JacobiSVD<Eigen::MatrixXd> svd(A, Eigen::ComputeFullU | Eigen::ComputeFullV);

        Eigen::Matrix<double,3,3,Eigen::RowMajor> fpre(svd.matrixV().col(8).data());

        Eigen::JacobiSVD<Eigen::Matrix3d> svd2(fpre, Eigen::ComputeFullU | Eigen::ComputeFullV);

        Eigen::Vector3d w = svd2.singularValues();
        w.z() = 0.;

        return svd2.matrixU() * Eigen::DiagonalMatrix<double,3>(w) * svd2.matrixV().transpose();
    }

    class FHTest{
    public:
        FHTest(){}
        ~FHTest(){}

        void FTest(){
            std::vector<cv::Point2f>kpts1(8,cv::Point2f(0., 0.));
            std::vector<cv::Point2f>kpts2(8,cv::Point2f(0., 0.));
            kpts1[0] = (cv::Point2f(-0.849647, -1.441589));
            kpts1[1] = (cv::Point2f(1.635149, -0.442614));
            kpts1[2] = (cv::Point2f(0.078962, 0.665838));
            kpts1[3] = (cv::Point2f(1.956590, 0.775309));
            kpts1[4] = (cv::Point2f(1.696376, -0.442614));
            kpts1[5] = (cv::Point2f(-0.278195, -0.401558));
            kpts1[6] = (cv::Point2f(-1.334362, 1.199536));
            kpts1[7] = (cv::Point2f(-0.548615, 0.638467));

            kpts2[0] = (cv::Point2f(-0.833396, -2.577254));
            kpts2[1] = (cv::Point2f(1.799436, -1.030115));
            kpts2[2] = (cv::Point2f(0.163809, 0.508376));
            kpts2[3] = (cv::Point2f(2.104973, 0.674311));
            kpts2[4] = (cv::Point2f(1.831388, -1.026050));
            kpts2[5] = (cv::Point2f(-0.202101, -0.986291));
            kpts2[6] = (cv::Point2f(-1.294323, 1.273339));
            kpts2[7] = (cv::Point2f(-0.478487, 0.472137));

            auto F12 = computeF21(kpts1, kpts2);
            std::cout<<" Fn   "<<std::endl;
            std::cout<<F12<<std::endl;
            std::cout<<"---------------"<<std::endl;
        }


        void HTset(){
            std::vector<cv::Point2f>kpts1(8,cv::Point2f(0., 0.));
            std::vector<cv::Point2f>kpts2(8,cv::Point2f(0., 0.));
            kpts1[0] = (cv::Point2f(-0.660864, 0.214240));
            kpts1[1] = (cv::Point2f(-0.395547, -0.442614));
            kpts1[2] = (cv::Point2f(-0.905771, 0.214240));
            kpts1[3] = (cv::Point2f(-2.033372, 0.255297));
            kpts1[4] = (cv::Point2f(1.920876, 0.652152));
            kpts1[5] = (cv::Point2f(-0.987408, 1.199536));
            kpts1[6] = (cv::Point2f(1.701478, 0.939523));
            kpts1[7] = (cv::Point2f(-0.074105, -0.511042));

            kpts2[0] = (cv::Point2f(-0.592076, -0.119070));
            kpts2[1] = (cv::Point2f(-0.322840, -1.045722));
            kpts2[2] = (cv::Point2f(-0.843877, -0.120485));
            kpts2[3] = (cv::Point2f(-2.030222, -0.059349));
            kpts2[4] = (cv::Point2f(2.069906, 0.515961));
            kpts2[5] = (cv::Point2f(-0.933472, 1.262233));
            kpts2[6] = (cv::Point2f(1.854246, 0.911037));
            kpts2[7] = (cv::Point2f(0.007088, -1.139396));

            auto H12 = computeH21(kpts1, kpts2);
            std::cout<<" Hn   "<<std::endl;
            std::cout<<H12<<std::endl;
            std::cout<<"---------------"<<std::endl;
        }

    };


}


#endif
