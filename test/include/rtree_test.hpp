/****************************************************************************************
 * @FilePath: rtree_test.hpp
 * @Author: Taber.wu
 * @Date: 2024-07-30 17:20:14
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-08-01 13:50:59
 * @Copyright: 2024 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef __RTREE_TEST_HPP__
#define __RTREE_TEST_HPP__

#include <boost/geometry.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/geometry/algorithms/reverse.hpp>

#include <Eigen/Eigen>
#include <Eigen/Core>
#include <vector>
#include <iostream>
#include "rotation.hpp"

#include <chrono>
#include <set>

namespace rtree{
typedef boost::geometry::model::point<float, 2, boost::geometry::cs::cartesian>  Point; 
typedef boost::geometry::model::box<Point> box;

typedef std::pair<Point, uint32_t> value;
typedef boost::geometry::index::rtree<rtree::value, boost::geometry::index::quadratic<16>> Rtree;


typedef boost::geometry::model::polygon<Point> Polygon;
typedef boost::geometry::model::linestring<Point> Linestring;
typedef boost::geometry::strategy::buffer::end_flat EndFlat;
typedef boost::geometry::strategy::buffer::distance_symmetric<float> ExpandR;
typedef boost::geometry::strategy::buffer::join_round JoinRound;
typedef boost::geometry::strategy::buffer::point_circle PointCircle;
typedef boost::geometry::strategy::buffer::side_straight SideStraight;
typedef boost::geometry::model::multi_linestring<Linestring> MutiLinestring;
typedef boost::geometry::model::multi_polygon<Polygon> MutiPolygon;


struct TrajData{
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    Eigen::Vector3d translation;
    Eigen::Quaterniond rotation;
    TrajData(){
        translation = Eigen::Vector3d::Zero();
        rotation = Eigen::Quaterniond::Identity();
    }
    TrajData(const Eigen::Vector3d t, const Eigen::Quaterniond q){
        translation = t;
        rotation = q;
    }

};

class RtreeTest{
public:
    RtreeTest(){}
    ~RtreeTest() = default;

    struct Ptcampare{
        bool operator()(const Point &k1, const Point &k2)const{
            if(k1.get<1>() == k2.get<1>())
                return k1.get<0>() < k2.get<0>();
            return  k1.get<1>() < k2.get<1>();
        }
    };


    void test(){
        auto tree = std::make_shared<Rtree>();
        tree->insert(std::make_pair(rtree::Point(0.1, 0.3), 1));
        tree->insert(std::make_pair(rtree::Point(0.0, 0.), 5));
        tree->insert(std::make_pair(rtree::Point(0.0, 0.1), 5));
        tree->insert(std::make_pair(rtree::Point(0.1, 0.1), 8));
        tree->insert(std::make_pair(rtree::Point(1.0, 0.5), 5));
        tree->insert(std::make_pair(rtree::Point(2.0, 0.), 5));
        tree->insert(std::make_pair(rtree::Point(3.1, 0.), 10));
        tree->insert(std::make_pair(rtree::Point(5.0, 0.2), 5));
        tree->insert(std::make_pair(rtree::Point(6.0, 1.), 5));
        tree->insert(std::make_pair(rtree::Point(8.1, -0.2), 1));
        tree->insert(std::make_pair(rtree::Point(9.0, -0.1), 5));
        tree->insert(std::make_pair(rtree::Point(10.0, -0.1), 5));


        rtree::Point pa(-0.1, -0.2);
        rtree::Point pb(9.0, 0.1);
        rtree::box query_box(pa, pb);
        std::vector<rtree::value> result;
        tree->query(boost::geometry::index::within(query_box), std::back_inserter(result));
        printf("range [0~9, -0.1~0.1] size %lu\n", result.size());
        for(size_t i = 0; i < result.size(); i++){
            printf("p (%.3f, %.3f) v %u\n", result[i].first.get<0>(), result[i].first.get<1>(), result[i].second);
        }

        rtree::Point pa1(100.0, -0.1);
        rtree::Point pb1(900.0, 0.1);
        rtree::box query_box1(pa1, pb1);
        std::vector<rtree::value> result1;
        tree->query(boost::geometry::index::within(query_box1), std::back_inserter(result1));
        printf("range [0~9, -0.1~0.1] size %lu\n", result1.size());
        for(size_t i = 0; i < result1.size(); i++){
            printf("p (%.3f, %.3f) v %u\n", result1[i].first.get<0>(), result1[i].first.get<1>(), result1[i].second);
        }
    }

    void collisioncheck(){

        std::vector<TrajData> traj;
        traj.push_back(TrajData(Eigen::Vector3d(0.0, 0.0, 0), Eigen::Quaterniond(1., 0., 0., 0.)));
        traj.push_back(TrajData(Eigen::Vector3d(1.0, 0.0, 0), Eigen::Quaterniond(1., 0., 0., 0.)));
        traj.push_back(TrajData(Eigen::Vector3d(2.0, 0.0, 0), Eigen::Quaterniond(1., 0., 0., 0.)));
        traj.push_back(TrajData(Eigen::Vector3d(2.38268, 0.07612, 0.0), Eigen::Quaterniond(0.98078528, 0., 0., 0.195090322)));
        traj.push_back(TrajData(Eigen::Vector3d(2.5, 0.13397, 0.0), Eigen::Quaterniond(0.965925826, 0., 0.,0.258819045)));
        traj.push_back(TrajData(Eigen::Vector3d(2.8660, 0.5, 0.0), Eigen::Quaterniond(0.866025404, 0., 0.,0.5)));
        traj.push_back(TrajData(Eigen::Vector3d(3.0, 1.0, 0.0), Eigen::Quaterniond(0., 0., 0.,1.)));
        traj.push_back(TrajData(Eigen::Vector3d(3.0, 2.0, 0.0), Eigen::Quaterniond(0., 0., 0.,1.)));
        Linestring traj_string1, traj_string2;
        MutiLinestring muti_line ;
        for(size_t id = 0; id < traj.size(); id++){
            auto traj_pt = traj[id];
            float x = (float)traj_pt.translation.x();
            float y = (float)traj_pt.translation.y();
            double yaw = jsos::utility::quaternion2EeulerAngle(traj_pt.rotation)[2];
            float dx = (1.3 * sin(yaw) );
            float dy = (1.3* cos(yaw) );
            traj_string1.push_back(Point(x+dx,y+dy));
            traj_string2.push_back(Point(x-dx,y-dy));
           
        }
        muti_line.push_back(traj_string1);
        muti_line.push_back(traj_string2);
        // traj_string.push_back(rtree::Point(0.0, 0.0));
        // traj_string.push_back(rtree::Point(1.0, 0.0));
        // traj_string.push_back(rtree::Point(2.0, 0.0));
        // traj_string.push_back(rtree::Point(2.38268, 0.07612));
        // traj_string.push_back(rtree::Point(2.5, 0.13397));
        // traj_string.push_back(rtree::Point(2.8660, 0.5));
        // traj_string.push_back(rtree::Point(3.0, 1.0));
        // traj_string.push_back(rtree::Point(3.0, 2.0));

        // EndFlat end_strategy;
        // ExpandR distance_strategy(1.0);
        // JoinRound join_strategy;
        // PointCircle point_strategy;
        // SideStraight side_strategy;
        // MutiPolygon traj_area;
        // MutiLinestring muti_line ;
        // muti_line.push_back(traj_string1);
        // muti_line.push_back(traj_string2);


        // boost::geometry::buffer(muti_line, traj_area, distance_strategy,side_strategy,
        //         join_strategy, end_strategy, point_strategy);



        std::vector<rtree::Point> obs;
        obs.push_back(rtree::Point(-1.0, 0.0));
        obs.push_back(rtree::Point(0.0, 0.0));
        obs.push_back(rtree::Point(0.0, 1.0));
        obs.push_back(rtree::Point(2.0, 2.0));
        obs.push_back(rtree::Point(3.0, 0.0));
        obs.push_back(rtree::Point(3.0, 2.1));
        obs.push_back(rtree::Point(105.0, 200.1));
        
        // std::set<Point,Ptcampare> outers;
        // for(auto pt: traj_area[0].outer()){
        //     outers.insert(pt);
        // }

        // Polygon hull;
        // for(auto pt : outers){
        //     hull.outer().push_back(pt);
        //     // printf("(%.3f, %.3f)\n", pt.get<0>(), pt.get<1>());
        // }

        auto hull = trajHull(muti_line);
        for(auto pt : hull.outer()){
            printf("(%.3f, %.3f)\n", pt.get<0>(), pt.get<1>());
        }
        printf("-----------------\n");
        for(auto ob :obs){
            auto start_stamp = std::chrono::system_clock::now();
            boost::geometry::de9im::mask mask("FF*FF****");
            auto relationship = boost::geometry::relate(ob, hull, mask);
            auto end_stamp = std::chrono::system_clock::now();
            uint64_t run_durion = std::chrono::duration<double, std::micro>(end_stamp- start_stamp).count();


           printf("(%.3f, %.3f)  relaship %d \n", ob.get<0>(), ob.get<1>(), (int)relationship);
            
          printf("ship use %lu micros \n", run_durion);
        //   printf("%d %d %d\n", relationship[0], relationship[1], relationship[2]);
        //   printf("%d %d %d\n", relationship[3], relationship[4], relationship[5]);
        //   printf("%d %d %d\n", relationship[6], relationship[7], relationship[8]);
        }
                
    }



protected:
     Polygon trajHull(const MutiLinestring &line_strings)
    {
        Polygon hull;
        hull.clear();
        for(size_t k = 0; k < line_strings.size(); k++){
           Point connect_pta, connect_ptb, connect_middle;
            if(k < line_strings.size() /2){
                boost::geometry::append(hull.outer(), line_strings[k]);
                connect_pta = line_strings[k].back();
            }
            else{
                Linestring reserve_line;
                reserve_line.assign(line_strings[k].rbegin(), line_strings[k].rend());
                boost::geometry::append(hull.outer(), reserve_line);
                connect_pta = line_strings[k].front();
            }
            
            if(k+1 < line_strings.size() /2){
                connect_ptb = line_strings[k+1].front();
            }
            else if(k+1 == line_strings.size()){
                connect_ptb = line_strings[0].front();
            }
            else {
                connect_ptb = line_strings[k+1].back();
            }
            connect_middle =Point((int)((connect_pta.get<0>() + connect_ptb.get<0>())/2), (int)((connect_pta.get<1>() + connect_ptb.get<1>())/2));
         
            boost::geometry::append(hull.outer(), connect_middle);


        }
        return hull;
    }


};

}

#endif