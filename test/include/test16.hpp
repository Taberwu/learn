/****************************************************************************************
 * @FilePath: test16.hpp
 * @Author: Taber.wu
 * @Date: 2023-04-04 09:21:18
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-04-17 10:00:28
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef TEST_SITEEN_HPP_
#define TEST_SITEEN_HPP_
#include <bitset>
#include <memory>
#include <vector>
#include <iomanip>
 struct Cantrans{
    public:
        std::uint8_t    buf[8] = {0};
        int mod = 0;
        int length = 64;
        template<typename T>
        const T getData(int tag, bool is_intelmod){
            auto data = static_cast<T>(buf[0]);
            if(is_intelmod){
                for(int i = 1; i < 8; i++){
                    if(i > (length+mod)/8) break;
                    data += (buf[i] << (i * 8 - mod));
                }
            }
            else{
                data = static_cast<T>(buf[7-tag]);
                int j = 1;
                for(int i = 8-tag; i < 8; i++){
                    data += (buf[i] << (j * 8 - (length+mod-8*tag)));
                    j++;
                }
            }       
            // std::cout<<"trans :"<<data<<"    "<<std::hex<<(int64_t)data;
            // std::cout<<std::dec<<std::defaultfloat<<std::endl;    
            return static_cast<T>(data);
        }
    };

    union CanTransBit {
        std::uint64_t   unsigned_data;
        std::uint8_t    buffer[8];
        std::bitset<sizeof(std::uint64_t) * 8> bit_set;
        CanTransBit(){
            unsigned_data = 0;
        }
    };

    int64_t bin2int(std::bitset<sizeof(int64_t) * 8>  bin , const uint8_t& length ){
    if(length == 8 || length == 16 || length == 32 || length == 64)
        return bin.to_ulong();
    int64_t data = 0;
    if(bin[length-1]){
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

    
  template <typename T, typename D> 
    void packSignal(T value, int startbit, int length,  std::vector<D> &data,
                    double offset=0.0, double factor=1.0, bool is_intelmod=true){
        std::uint64_t unit_data = static_cast<std::uint64_t>((value - offset)/factor + 0.5f);

        int index = startbit/8;
        int mod = startbit%8;
        for(int i = 0; i < length + mod ; i += 8){
            if(index+i/8 > (int)data.size()-1){
                std::cerr<<"must resize msg data size before pack signal !"<<std::endl;
                break;
            }
            D tmpdata = 0;
            std::uint64_t valid = (1 << ((8 +  i > length + mod)? length : (8 -mod + i ))) - (1 << ((i < mod)? 0 : i-mod));
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



//default Intel model 
template <typename T> 
const T unPackSignalBitset(const int startbit, const int length, const std::bitset<64> &bit_set, 
            double offset = 0.0, double factor = 1.0, bool is_signed = false, bool is_intelmod = true){
    std::bitset<64> signal_bitset("0");
    if(is_intelmod){
        for(int i = 0; i < length; i++){
            signal_bitset[i] = bit_set[startbit + i];
        }
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
        return static_cast<T>((double)signal_raw * factor + offset);
    }
    auto unsigned_data = signal_bitset.to_ulong();
    // std::cout<<"bitset :"<<signal_bitset<<std::endl;
    // std::cout<<unsigned_data<<"    "<<std::hex<<(int64_t)unsigned_data;
    // std::cout<<std::dec<<std::defaultfloat<<std::endl;
    return static_cast<T>((double)unsigned_data * factor + offset);
}   


 template <typename T, typename D> 
    const T unPackSignal(const int startbit, const int length, const std::vector<D> data,
                                  const double offset=0.0, const double factor=1.0, bool is_intelmod=true){
        Cantrans trans;
        int index = startbit /8;
        int mod = startbit % 8;
        trans.length = length;
        trans.mod = mod;
        int tag = 0;
        for(int i = 0; length - i + mod > 0; i += 8){
            tag = i;
            auto tmp = data[index + i/8];
            if(i == 0){
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
        tag = tag / 8;
        auto output = static_cast<double>(trans.getData<std::uint32_t>(tag, is_intelmod) * factor + offset) ;
        return static_cast<T>(output);
    }



#endif

