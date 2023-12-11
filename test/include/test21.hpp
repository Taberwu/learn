/****************************************************************************************
 * @FilePath: test21.hpp
 * @Author:         Taber.wu
 * @Date:           2023-08-11 13:16:00
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-08-11 14:09:59
 * @Copyright:      2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef _GFLAGS_TEST_HPP_
#define _GFLAGS_TEST_HPP_

#include <gflags/gflags.h>
#include <string>
#include <vector>

class GflagsTest{
public:
    GflagsTest(){
        param_list.clear();
    }
    ~GflagsTest() = default;
    bool init(int argc, const char* argv[]){
        if(argc < 1){
            printf("use default param !\n");
            return false;
        }
        else{
            printf("argc: %d\n",argc);
            param_list.assign(argc,"");
            for(int i=0; i<argc; i++){
                param_list[i] = argv[i];
                printf("%d:  %s\n",i, param_list[i].c_str());
            }
        }
        return true;
    }
    void test(){
        if(param_list.size() == 0){
            printf("test no param\n");
        }
        else if(param_list.size() < 2){
            printf("test first param [%s]\n", param_list[0].c_str());
        }
        else{
             printf("test second param [%s]\n", param_list[1].c_str());
        }
    }
   
protected:
    std::vector<std::string> param_list;

};


#endif
