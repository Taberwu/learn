/****************************************************************************************
 * @FilePath: fucntion_bind.hpp
 * @Author: Taber.wu
 * @Date: 2024-05-08 11:11:15
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-05-08 11:28:20
 * @Copyright: 2024 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef __FUNCTION_BIND_HPP__
#define __FUNCTION_BIND_HPP__

#include <memory>
#include <string>
#include <functional>
#include <vector>

namespace function{
    class FunBind{
    public:
        FunBind(){}
        ~FunBind(){}

        void test(){
            auto testfunc = std::bind(&FunBind::funA, this,std::placeholders::_1, std::placeholders::_2);
            auto str = std::make_shared<std::string>("function_bind");
            for(int i = 0; i < 8; i++){
                testfunc(i, str);
            }
        }
        void test2(){
            handler_ = std::bind(&FunBind::funA, this,std::placeholders::_1, std::placeholders::_2);
            auto str = std::make_shared<std::string>("function_bind");
            for(int i = 9; i < 16; i++){
                handler_(i, str);
            }
        }
    protected:
        void funA(const int a, std::shared_ptr<std::string> str){
            if(a > str->length()-1){
                printf("%d  char  NULL\n", a);
            }
            else{
                printf("%d char %c\n", a, str->at(a));
            }
        }

    private:
        std::function<void(const int , std::shared_ptr<std::string> )> handler_;

    };
}

#endif