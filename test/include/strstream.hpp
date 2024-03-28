/****************************************************************************************
 * @FilePath: strstream.hpp
 * @Author: Taber.wu
 * @Date: 2024-03-12 10:50:34
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-03-12 10:59:05
 * @Copyright: 2024 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef _STR_STREAM_HPP_
#define _STR_STREAM_HPP_

#include <string>
#include <sstream>
#include <iostream>
#include <Eigen/Eigen>
#include <Eigen/Core>

#include "rotation.hpp"

namespace strsstream{
    class Sstm{
        public:
           Sstm(){} 
           ~Sstm() = default;
           void test(){
                Eigen::Quaterniond m_q_ex(0.999967,0.00307366,0.00572324,0.00496534);
                Eigen::Vector3d m_t_ex(-0.105895, -0.00829935,0.0271156);
                std::stringstream stereo_out_;
                stereo_out_.str("stereo extrinsicParams: \n");
                std::string context = "translation: \n";

                context += " X: "; context += std::to_string(m_t_ex.x()); context += "\n";
                context += " Y: "; context += std::to_string(m_t_ex.y()); context += "\n";
                context += " Z: "; context += std::to_string(m_t_ex.z()); context += "\n";
                stereo_out_ << context;
            
                context = "Quaterniond:\n";
                context += " W: "; context += std::to_string(m_q_ex.w()); context += "\n";
                context += " X: "; context += std::to_string(m_q_ex.x()); context += "\n";
                context += " Y: "; context += std::to_string(m_q_ex.y()); context += "\n";
                context += " Z: "; context += std::to_string(m_q_ex.z()); context += "\n";
                stereo_out_ << context;
                
                context = "RotationMatrix:\n";
                stereo_out_ << context;
                stereo_out_ << m_q_ex.toRotationMatrix()<<std::endl;
            
                auto euler = jsos::utility::quaternion2EeulerAngle(m_q_ex);
                context = "Euler:\n";
                context += " Roll: "; context += std::to_string(euler.x()); context += "\n";
                context += " Pitch: "; context += std::to_string(euler.y()); context += "\n";
                context += " Yaw: "; context += std::to_string(euler.z()); context += "\n";
                stereo_out_ << context;

                std::string tmp1 = stereo_out_.str();
                std::cout<< tmp1;
           }
    };
}

#endif