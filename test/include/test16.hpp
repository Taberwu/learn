/****************************************************************************************
 * @FilePath: test16.hpp
 * @Author: Taber.wu
 * @Date: 2023-04-04 09:21:18
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-04-04 10:20:53
 * @Copyright: 2023 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************/
#ifndef TEST_SITEEN_HPP_
#define TEST_SITEEN_HPP_
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
            return static_cast<T>(data);
        }
};


class CanTrans{
public:
    CanTrans(){}
    ~CanTrans()=default;
 template <typename T, typename D> 
 static   const T unPackSignal(const int startbit, const int length, const std::vector<D> data,
                                  const double offset=0.0 , const double factor=1.0, bool is_intelmod=true){
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
        auto output = static_cast<double>(trans.getData<std::uint32_t>(tag, is_intelmod)* factor + offset) ;
        return static_cast<T>(output);
    }

    template <typename T, typename D> 
    static void packSignal(const T value, const int startbit, const int length,  std::vector<D> &data,
                    const double offset=0.0 ,const double factor=1.0, bool is_intelmod=true){
        auto test = (value  - offset)/factor;
        std::uint64_t unit_data = static_cast<std::uint64_t>((value  - offset)/factor + 0.5f);
        std::cout<<" transout uint "<<std::hex<<std::setw(2)<<std::setfill('0')<<unit_data<<" "<<(uint)test <<std::endl;
        int index = startbit/8;
        int mod = startbit%8;
        for(int i = 0; i < length + mod ; i += 8){
            if(index+i/8 > (int)data.size()-1){
                std::cerr<<"must resize msg data size before pack signal !"<<std::endl;
                break;
            }
            D tmpdata = 0;
            int valid = (1 << ((8 + 8* i > length + mod)? length : (8 -mod + 8*i ))) - (1 << ((8*i < mod)? 0 : 8*i-mod));
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

#endif

