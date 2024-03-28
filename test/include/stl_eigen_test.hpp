/****************************************************************************************
 * @FilePath: stl_eigen_test.hpp
 * @Author: Taber.wu
 * @Date: 2024-03-21 17:55:45
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-03-22 09:40:55
 * @Copyright: 2024 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#include <map>
#include <vector>
#include <string>
#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include <Eigen/StdVector>
typedef std::map<std::uint64_t, Eigen::Vector4f> DataA;
namespace stleigen{
    class StlEigen{
        public:
           StlEigen(){}
           ~StlEigen(){} 
            struct SensorAData{
                std::uint64_t timestamp;
                std::string id;
                DataA datas;
            };
        void test(){
            SensorAData sensor1;
            sensor1.timestamp = 0;
            sensor1.datas.insert({1, Eigen::Vector4f(1., 2., 4., 7.)});
            sensor1.datas.insert({2, Eigen::Vector4f(2, 4., 8., 14.)});

            printf("%lu  %.2f %.2f %.2f\n", sensor1.timestamp,sensor1.datas[1][0],sensor1.datas[2][2], sensor1.datas[2][0]);
        }
    };
}