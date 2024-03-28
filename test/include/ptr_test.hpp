/****************************************************************************************
 * @FilePath: ptr_test.hpp
 * @Author: Taber.wu
 * @Date: 2024-02-01 14:45:35
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-02-01 15:08:50
 * @Copyright: 2024 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef _PTR_TEST_HPP_
#define _PTR_TEST_HPP_
#include <iostream>
class PtrTest{
public:
    PtrTest(){}
    ~PtrTest(){}
    void test(){
        int a = 1;
        
        int b = 3;
        int c = 5;
        const int *ptr1 = &a;
        int const *ptr2 = &b;
        int* const ptr3 = &b;
        auto a_p = ptr1;
        auto b_p = ptr2;
        auto c_p = ptr3;
        printf("value [%d %d %d] ptr[%lu %lu %lu]\n", *ptr1, *ptr2, *ptr3, a_p, b_p, c_p);
        b = 6;
        a_p = ptr1;
        b_p = ptr2;
        c_p = ptr3;
        printf("value [%d %d %d] ptr[%lu %lu %lu]\n", *ptr1, *ptr2, *ptr3, a_p, b_p, c_p);
        ptr1 = &c;
        a_p = ptr1;
        b_p = ptr2;
        c_p = ptr3;
        printf("value [%d %d %d] ptr[%lu %lu %lu]\n", *ptr1, *ptr2, *ptr3, a_p, b_p, c_p);
        *ptr3 = 9;
        a_p = ptr1;
        b_p = ptr2;
        c_p = ptr3;
        printf("value [%d %d %d] ptr[%lu %lu %lu]\n", *ptr1, *ptr2, *ptr3, a_p, b_p, c_p);
    }
};

#endif
