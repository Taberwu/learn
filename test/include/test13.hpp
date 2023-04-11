/****************************************************************************************
 * @FilePath: test13.hpp
 * @Author: Taber.wu
 * @Date: 2023-03-01 09:38:30
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-03-08 09:26:11
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef TEST_CAN_13_HPP_
#define TEST_CAN_13_HPP_

#include <istream>
#include <vector>
#include <unistd.h>
#include <iomanip>

union Cantrans
{
    std::uint64_t data = 0;

    std::uint8_t buf[8];
};


class Canparse{
public:
    Canparse(){}
    ~Canparse() = default;

    template <typename T, typename D>
    static const T unPackSignal(const int startbit, const int length, const std::vector<D> data,
                                  const std::uint32_t offset = 0.0, const double factor = 1.0, bool is_intelmod = true){
        Cantrans trans;
        trans.data = 0;
        int index = startbit /8;
        int mod = startbit % 8;
        for(int i = 0; length - i > 0; i += 8){
            auto tmp = data[index + i/8];
            if(i==0){
                   tmp = tmp >> mod;
               }
            int reservebit = (length - i+ mod > length) ? length : length - i+ mod;
            if(reservebit < 8){
                 tmp = (tmp&((1 << reservebit)-1));
            }
            if(is_intelmod){
                trans.buf[i/8] = tmp;
            }
            else{
                trans.buf[7-i/8] = tmp;
            } 
        }
        std::cout<<std::hex<<trans.data<<std::endl;
        auto output = static_cast<double>(trans.data + offset) * factor;
        return static_cast<T>(output);
    }

    template<typename T, typename D>
    static void packSignal(const T value, const int startbit, const int length,std::vector<D> &data,
                  const std::uint32_t offset = 0, const double factor = 1.0, bool is_intelmod = true){
        std::uint64_t unit_data = static_cast<std::uint64_t>(value / factor - offset);
       
        int index = startbit/8;
        int mod = startbit%8;
        for(int i = 0; i < length ; i += 8){
            if(index+i/8 > (int)data.size()-1){
                std::cerr<<"must resize msg data size before pack signal !"<<std::endl;
                break;
            }
            int valid = (1 << (i+8-mod)) - (1 << ((i>mod)? (i-mod) : 0));
            D input = 0;
            if(i == 0){
                input = static_cast<D>((unit_data & valid) << mod);
            }
            else{
               input = static_cast<D>((unit_data & valid) >> (i-mod));
            }
            if(is_intelmod)
                data[index+i/8] += input;
            else{
                data[index+(length - i -1)/8] += input;
            }
        }}

const std::int64_t adpatSignal(std::uint64_t data, const int length){
    
}


};

#endif