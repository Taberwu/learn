/****************************************************************************************
 * @FilePath: test20.hpp
 * @Author: Taber.wu
 * @Date: 2023-07-12 10:10:14
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-07-12 10:24:06
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef SWITH_TEST_HPP_
#define SWITH_TEST_HPP_

#include <iostream>

class SwitchTesT{
public:
    SwitchTesT(){}
    ~SwitchTesT() = default;
    void Test(int n){
        switch(n){
            case 1:
            case 3:
            case 4:
            case 5:
                printf("---------\n");
                break;
            case 2:{
                printf("==========\n");
                break;
            }
            case 6:
            case 7:
            case 8:
            case 10:{
                printf("*********\n");
                break;
            }
            case 9:
                printf(".............\n");
                break;
            default:    
                printf("##########\n");
                break;
        }

    }
};

#endif