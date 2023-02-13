/**
****************************************************************************************
 * @FilePath: test9.h
 * @Author: Taber.wu
 * @Date: 2023-02-10 16:56:52
 * @LastEditors: 
 * @LastEditTime: 2023-02-10 16:56:53
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************
*/
#ifndef _TEST_NIGHT_H_
#define _TEST_NIGHT_H_

#include <typeinfo>
#include <cxxabi.h>
#include <string>

template<typename T>
static const std::string getTypename(){
    std::string str = abi::__cxa_demangle(typeid(T).name(),NULL,NULL,NULL);
    return str;
}

#endif