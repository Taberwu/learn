/****************************************************************************************
 * @FilePath: stringtest.hpp
 * @Author: Taber.wu
 * @Date: 2023-10-24 16:11:24
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-10-24 16:23:58
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#include <string>
#include <iostream>

namespace stringtest{
    class Stringcut{
        public:
        Stringcut(const std::string str):used_str_(str)
        {}
        ~Stringcut() = default;

        void test(){
            auto it1 = used_str_.find_last_of('/');
            auto tmp_str = used_str_.substr(it1+1);
            std::cout<<"file name : "<<tmp_str<<std::endl;
            auto it2 = tmp_str.find_last_of('.');
            std::string res = tmp_str.substr(0, it2);
            std::cout<<" file : "<<res<<std::endl;

        }

        private:
            std::string used_str_;
    };
}