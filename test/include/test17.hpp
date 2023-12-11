/****************************************************************************************
 * @FilePath: test17.hpp
 * @Author: Taber.wu
 * @Date: 2023-04-11 15:00:48
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-04-12 10:52:31
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef _TEST_SEVENTEEN_HPP_
#define _TEST_SEVENTEEN_HPP_

#include <iomanip>
#include <bitset>

namespace SevenTeenTest{

    template<typename T, typename D>
    static  const T unPack(int index, int length ,const D *data){
            T sum = 0;
            for(int i =0; i<3;i++){
                if(index + i > length -1) break;
                sum += static_cast<T>(data[index+i]);
            }

            return sum;
            
    }

    union Signeddata{
        std::int16_t s_data ;
        std::uint16_t u_data;
        uint8_t buffer[2] ={0};
    };

    union Transcan{
        std::uint64_t   unsigned_data;
        std::uint8_t    buffer[8];
        std::bitset<sizeof(std::uint64_t) * 8> bit_set;
        Transcan(){
            unsigned_data = 0;
        }
    };


 
}
#endif