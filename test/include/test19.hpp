/****************************************************************************************
 * @FilePath: test19.hpp
 * @Author: Taber.wu
 * @Date: 2023-06-25 08:38:11
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-07-04 16:50:51
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef CAN_DATA_SIGNED_TEST_HPP_
#define CAN_DATA_SIGNED_TEST_HPP_
#include <cmath>
#include <string>
#include <bitset>
#include <vector>
#include <iostream>

namespace NighteenTest{

union CanTrans {
    std::uint64_t   unsigned_data;
    std::uint8_t    buffer[8];
    std::bitset<sizeof(std::uint64_t) * 8> bit_set;
    CanTrans(){
        unsigned_data = 0;
    }
};

class SignDataParse{
public:
    SignDataParse(){}
    ~SignDataParse() =default;
    void parse_candata(std::uint8_t *can_data){
        CanTrans can_signal;
        for(int i=0; i<8; i++){
            can_signal.buffer[i] = static_cast<std::uint8_t>(*(can_data+i));
        }
        std::cout<<"data bitset :"<< can_signal.bit_set.to_string()<<std::endl;
        std::vector<float> param;
        param.assign(4, 0.);

        param[3] = unPackSignal<float>(0, 11, can_signal.bit_set, 0, 0.01, true);
        param[2] = unPackSignal<float>(11, 11, can_signal.bit_set, 0, 0.0009765625, true);
        param[1] = unPackSignal<float>(22, 16, can_signal.bit_set, 0, 9.77e-7, true);
        param[0] = unPackSignal<float>(38, 17, can_signal.bit_set, 0, 3.7252902985e-9, true);
        auto time_to_cross = unPackSignal<float>(55, 8, can_signal.bit_set, 0, 0.01);
        printf("output data [%.5f %.5f %.7f %.7f], %.3f\n",param[3], param[2], param[1], param[0], time_to_cross);
    }

protected:
    /* 使用bitset将二进制 转为 int类型 */
/* length 为二进制数据长度 */
static int64_t bin2int(std::bitset<sizeof(int64_t) * 8>  bin , const uint8_t& length ){
    // if(length == 8 || length == 16 || length == 32 || length == 64)
    //     return bin.to_ulong();
    int64_t data = 0;
    if(!bin[length-1]){
        data = (int64_t)bin.to_ulong();
    }else{
        bool if_down = true;
        uint8_t index = 0;
        // 末尾-1
        while(if_down && index < length){
            if_down = !bin[index];
            bin[index] = ~bin[index];
            index++;
        }

        // 取反
        index = 0;
        while(index < length)
            bin.flip(index++);

        // 得到负数
		// std::cout << bin << std::endl;
        data = -(int64_t)bin.to_ulong();

    }
    return data;
}

//default Intel model 
template <typename T> 
static const T unPackSignal(const int startbit, const int length, const std::bitset<64> &bit_set, 
            double offset = 0.0, double factor = 1.0, bool is_signed = false, bool is_intelmod = true){
    std::bitset<64> signal_bitset("0");
    if(is_intelmod){
        for(int i = 0; i < length; i++){
            signal_bitset[i] = bit_set[startbit + i];
        }
        std::cout<<"signal bitset :"<<signal_bitset.to_string()<<std::endl;
    }
    else{
        int j = startbit;
        for(int i = 0; i < length; i++){
            signal_bitset[length - i -1] = bit_set[j];
            j += (j%8 == 0)? 15 : -1;
        }
    }
    if(is_signed){
        auto signal_raw = bin2int(signal_bitset, length);
        std::cout<<std::hex<<"raw int data "<<signal_raw<<std::endl;
        return static_cast<T>((double)signal_raw * factor + offset);
    }
    auto unsigned_data = signal_bitset.to_ulong();
    return static_cast<T>((double)unsigned_data * factor + offset);
}

template <typename T, typename D> 
static void packSignal(T value, int startbit, int length,  D *data,
                double offset = 0.0 ,double factor = 1.0, bool is_intelmod = true){
    std::uint64_t unit_data = static_cast<std::uint64_t>((value - offset)/factor + 0.5f);
    int index = startbit/8;
    int mod = startbit%8;
    for(int i = 0; i < length + mod ; i += 8){
        if(index+i/8 > (int)data.size()-1){
            printf("must resize msg data size before pack signal !\n");
            break;
        }
        D tmpdata = 0;
        int valid = (1 << ((8 +  i > length + mod)? length : (8 -mod + i ))) - (1 << ((i < mod)? 0 : i-mod));
        if(i == 0){
           tmpdata = static_cast<D>((unit_data & valid) << mod);
        }
        else{
            tmpdata = static_cast<D>((unit_data & valid) >> (i-mod));
        }
        if(is_intelmod){
             data[index+i/8] += tmpdata;
        }
        else{
            data[index+(length - i -1)/8] += tmpdata;
        }
}}
};
}
#endif