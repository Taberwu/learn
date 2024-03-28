/**
****************************************************************************************
 * @FilePath: test.cpp
 * @Author: Taber.wu
 * @Date: 2022-10-09 11:24:39
 * @LastEditors: 
 * @LastEditTime: 2022-10-09 11:24:39
 * @Copyright: 2022 JOYSON CO.,LTD. All Rights Reserved.
 * @Descripttion: 
****************************************************************************************
*/
#include <test1.h>
#include <test2.h>
#include <test3.h>
#include <test4.h>
#include <cmath>
#include <caculateFH.hpp>
// #include <restart_thread.hpp>
// #include <stl_eigen_test.hpp>
// #include <bind_test.hpp>
// #include <ptr_test.hpp>
// #include <test5.h>
// #include <test6.h>
// #include <test7.h>
// #include <test8.h>
// #include <test9.h>
// #include <test10.hpp>
// #include <test11.h>
// #include <test12.hpp>
// #include <test13.hpp>
// #include <test14.hpp>
// #include <test15.hpp>
// #include <test16.hpp>
// #include <test17.hpp>
// #include <test18.hpp>
// #include <test19.hpp>
// #include <test20.hpp>
// #include <test21.hpp>
#include <test22.hpp>
// #include <test23.hpp> 
#include <test24.hpp>
#include <test25.hpp>
#include <test26.hpp>
#include <test27.hpp>
// #include <imu_test.hpp>
#include "stringtest.hpp"
#include <random>
#include <thread>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <vector>
#include <limits>
#include <rotation.hpp>
// #include <vector_insert.hpp>
// #include <vel_rotation.hpp>
#include <strstream.hpp>
#include <typetest.hpp>

// void fun(int &a, int &b){
//     std::cout<<"a ,b ("<<a<<","<<b<<")"<<std::endl;
// }

// void f1( TestEi::DataA& data){
//     auto a = data;
//     a.run();
//     std::cout<<"----------"<<std::endl;
// }

// void f2( TestEi::DataB& data){
//     auto b = data;
//     b.run();
//     std::cout<<"==========="<<std::endl;
// }

// template<typename T>
// void mycout (double a){
//     T b = (T)a;
//     std::cout<<b<<std::endl;
// }

// using namespace Test;

// template <typename scalar>
// static Eigen::Quaternion<scalar> eulerAngle2Quaternion(const Eigen::Matrix<scalar, 3, 1>& euler)
// {
//     // https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
//     scalar yaw = euler.z();
//     scalar pitch = euler.y();
//     scalar roll = euler.x();

//     scalar cy = std::cos(yaw * 0.5);
//     scalar sy = std::sin(yaw * 0.5);
//     scalar cp = std::cos(pitch * 0.5);
//     scalar sp = std::sin(pitch * 0.5);
//     scalar cr = std::cos(roll * 0.5);
//     scalar sr = std::sin(roll * 0.5);

//     scalar w = cy * cp * cr + sy * sp * sr;
//     scalar x = cy * cp * sr - sy * sp * cr;
//     scalar y = sy * cp * sr + cy * sp * cr;
//     scalar z = sy * cp * cr - cy * sp * sr;
//     return Eigen::Quaternion<scalar>(w, x, y, z);
// }

int main(int argc, const char *argv[]){
    // auto imuMsg = std::make_shared<LocalizationMessage>();
    // auto rtkMsg = std::make_shared<InspvaMessage>();
    // auto wheMsg = std::make_shared<WheelMessage>() ;
    // auto locQue = std::make_shared<LocalizationQue>();
    // auto locVec = std::make_shared<LocalizationVec>();
    // std::shared_ptr<std::map<int,std::deque<Dataware>>> data;

    // std::condition_variable_any data_sts;
    // std::mutex locker;
    // uint64_t data_ready = 0;
    // auto  imu_data = std::make_shared<LocQue>();

    // // std::condition_variable_any data_sts;
    // std::mutex locker;
    // // uint64_t data_ready = 0;
    // // auto  imu_data = std::make_shared<LocQue>();

    //    if(i%2==1){
    //        wheMsg->setTimestamp(i*1000*1000);
    //        wheMsg->setRLPluse(random()%5);
    //        wheMsg->setRRPluse(random()%5);
    //        data[2].emplace_back(wheMsg);
    //    }
    //    if(i%3==2){
    //        rtkMsg->setTimestamp(i*1000*1000);
    //        rtkMsg->setLLA(Eigen::Vector3d(31.4356-i/1000.0,121.3425+i/1000.0,(random()%50)/25.0));
    //        data[3].emplace_back(rtkMsg);
    //    }
    
    // auto threadA = locQue->getInsertThread(imuMsg,&data_sts,&locker,imu_data,&data_ready);
    // auto threadB = locVec->converThread(data_sts,locker,imu_data,data_ready);
    // threadA.join();
    // std::cerr<<"in main data &&size "<<data_ready<<" "<<imu_data->size()<<std::endl;
    // threadB.detach();

    // usleep(10000);
    // }
    // std::vector<std::vector<double>> lanelines;
    // lanelines.clear();
    // lanelines.resize(4);
    // lanelines[1] = (std::vector<double>(8,1.0));
    // lanelines[3] = (std::vector<double>(11,-1.0));
    // for(int i = 0; i<lanelines.size();i++) 
    //     std::cout<<i<<"  "<<lanelines[i].size()<<std::endl;

    // std::array<int, 4> a;
    // std::array<int, 5> b;
    // std::cout<<"a :";
    // for(int i =0;i<a.size();i++){
    //     a[i] =  (random()%10);
    //     std::cout<<a[i]<<" ";
    // }
    // std::cout<<std::endl;
    // std::cout<<"b :";
    // for(int i =0;i<b.size();i++){
    //     b[i] =  (random()%10);
    //     std::cout<<b[i]<<" ";
    // }
    // std::cout<<std::endl;
    // auto ans = std::inner_product(a.begin(), a.end(), b.begin(),0,std::plus<int>(),[](int a ,int b){
    //     int  c = b-a;
    //     return c*c;
    // });
    // std::cout<<"ans  "<<ans<<std::endl;

    // std::array<Vec2D,8> a;
    // std::array<Vec2D,6> b;
    // std::vector<int> data_a = std::vector<int>(4,5);
    // Eigen::VectorXi input = Eigen::VectorXi::Random(data_a.size()+1);
    // std::cout<<"input ";
    // for(int i=0;i<input.size();i++){
    //     std::cout<<input[i]<<" ";
    // }
    // std::cout<<std::endl;
    // input -= Eigen::VectorXi::Ones(data_a.size()+1) * data_a[data_a.size()/2];
    // std::cout<<"output ";
    // for(int i=0;i<input.size();i++){
    //     std::cout<<input[i]<<" ";
    // }
    // std::cout<<std::endl;

    // std::vector<int>ans(8,1);
    // for(auto &data :ans){
    //     ++data;
    // }
    // std::cout<<"ans ";
    // for(auto data:ans) std::cout<<data<<" ";
    // std::cout<<std::endl;
    // std::string filepath = "/home/joyson/Desktop/tmp/zupt/";
    // std::string filename = "imu_static.txt";
    // std::string outfile = "imu_checkstatic.csv";
    // auto test3_imp = std::make_shared<TestThd>(filepath);
    // test3_imp->adjustTimestamp(1670985924747864703, filename, outfile);
    // auto test4 = std::make_shared<TestF>();
    // std::shared_ptr<std::vector<double>> test1 (new std::vector<double>(5,1.0));
    // auto clone_test = std::make_shared<std::vector<double>>(std::vector<double>(*test.get()));
    // for(int i = 0;i<test1->size();i++){
    //     test1->at(i) *= i;
    // }
    // for(int i = 0;i<test1->size();i++){
    //     std::cout<<test1->at(i)<<" ";
    // }
    // std::cout<<std::endl;
    // for(int i = 0;i<clone_test->size();i++){
    //     std::cout<<clone_test->at(i)<<" ";}
    // 
    // Eigen::Vector3d vec_omega (-0.00104691073939974, -0.00418729105814572, -0.00108035647618588);
    // Eigen::Vector3d vec_acce (-0.29283355228771, 0.337016259318792, 9.78620193280072);
    // auto t = vec_acce.cross(vec_omega);
    // std::cout<<"cross result :"<<std::endl;
    // std::cout<<t<<std::endl;
    // t /= t.norm();
    // std::cout<<" unit t:"<<std::endl;
    // std::cout<<t<<std::endl;
  
    // auto test_six = std::make_shared<TESTSX>(31.34532872, 5.0, Eigen::Vector3d(1, 0, 0));
    // test_six->test();

    // Eigen::Vector4d tmp = Eigen::Vector4d::Ones();
    // Eigen::Vector3d a(2.,3.,4.0);
    // double b = 5.0;
    // tmp = Eigen::Vector4d(a[0], a[1], a[2], b);
    // std::cout<<tmp.transpose()<<std::endl;

    // auto test2 = std::make_shared<Test::TestSev>();
    // test2->register_recorder("imu");
    // test2->register_recorder("raw");
    // double data_a2=-1.0;
    // int data_a3 =0;
    // std::int64_t data_timestamp;
    // std::thread th1 = std::thread([&](){
    //     int i =0;
    //     Test::DataA data_a;
    //     data_a.a1 = M_PI;
    //     data_a.a2 = 75.9268;
    //     data_a.a3 = 1234;
    //     data_a.a4 = {"imu"};
    //     data_a.sts = - 0b11100;
    //     std::string format_a = {"I: %llu, %.5f,%.1f,%d,%s %u\n"};
    //     while(i<80){
    //       auto t = std::chrono::steady_clock::now().time_since_epoch().count();
    //       data_a2 = data_a.a2;
    //       data_a3 = ++i;
    //       data_timestamp = t;
    //       test2->record("imu", format_a, t,data_a.a1, data_a.a2, data_a.a3, data_a.a4.c_str(), data_a.sts);
           
    //         usleep(10000);
    //     }
    // });

    //     std::thread th2 = std::thread([&](){
    //     int i =0;
    //     Test::DataB data_b;
    //     data_b.b1 = {"data_status"};
    //     data_b.sts = 0x1e;
    //     data_b.b2 = 4.;
    //     data_b.b3 = 125452;
    //     std::string format_a = {"V: %llu, %s %u %.1f%d\n"};
    //     std::string format_raw ={"R: %llu , %llu, %d  %.3f %s \n"};
    //     while(i<40){
    //       auto t = std::chrono::steady_clock::now().time_since_epoch().count();
    //       test2->record("imu", format_a, t,data_b.b1.c_str(), 
    //                     data_b.sts, data_b.b2, data_b.b3);
    //       if((data_a3/10)%2==0)  data_a2 *=-1.0;
    //       test2->record("raw", format_raw, data_timestamp, t,
    //                                      data_a3, data_a2, data_b.b1.c_str());
    //         i++;
    //         usleep(20000);
    //     }
    // });
    // th1.join();
    // th2.join();
   
    //  [&](const TestEi::DataA &data){
    //     std::cout<<"-----------"<<std::endl;
    // };

    
    // auto test3 = std::make_shared<TestEi::Sub>();
    // void (*data_Acallback) ( TestEi::DataA &data) = f1;
    // void (*data_Bcallback) ( TestEi::DataB &data) = f2;
    // void (*data_Bcallback)(const TestEi::DataB &data) = [&](const TestEi::DataB &data){
    //     std::cout<<"============="<<std::endl;
    // };
    // test3->register_handler(data_Acallback);
    // test3->register_handler(data_Bcallback);
    // test3->run();
    // std::vector<std::string>test_coll;

    // std::string test4 ={"R: 345267171171 121.54682821 31.2517907 3.163 152.34 1.362 -0.466"};
    // test_coll.push_back(test4);
    // std::string test2={"I: 0.0036 0.0013 1.4325 1.4352 0.6312 -9.7999"};
    // test_coll.push_back(test2);
    // std::uint64_t time_stamp;
    // double lat, lon;
    // float height,yaw,pitch,raw,we,wn,wu,ae,an,au;
    // for(auto &str :test_coll){
    //     auto tmp = str.substr(3,str.size()-3);
    //     if(str[0] == 'R'){
    //         std::stringstream ss(tmp);
         
    //         ss>>time_stamp>>lat>>lon>>height>>yaw>>pitch>>raw;
    //         std::cout<<std::setprecision(11)<<time_stamp<<" "<<lat<<" "<<lon<<" "<<height<<" "<<yaw<<" "<<pitch<<" "<<raw<<std::endl;
    //         std::cout<<"================"<<std::endl;
    //     }
    //     if(str[0] == 'I'){
    //         std::stringstream ss;
    //         ss.setf(std::ios::fixed);
    //         ss.precision(4);
    //         ss.str(tmp);
    //         ss>>we>>wn>>wu>>ae>>an>>au;
    //         std::cout<<we<<" "<<wn<<" "<<wu<<" "<<ae<<" "<<an<<" "<<au<<std::endl; 
    //     }
    // }
    // {
    // TestEi::DataB b (TestEi::Database(M_PI));
    // std::cout<<typeid(b).name()<<std::endl;
    // std::cout<<abi::__cxa_demangle(typeid(b).name(),NULL,NULL,NULL)<<std::endl;
    // }
    // // TestEi::DataA a (TestEi::Database(M_PI));
    // // std::cout<<typeid(a).name()<<std::endl;
    // std::cout<<"------------------"<<std::endl;
    // std::cout<<typeid(DatabasePtr).name()<<std::endl;
    // std::string strt = getTypeName<DatabasePtr>();
    // std::cout<<strt<<std::endl;



    // int i = 1;

    // auto int_pro = [&](int &data){
    //     data+=i;
    // };
    // auto hate_int = std::make_shared<test_thread::Hate<int>>("int", int_pro, 30);

    // auto float_pro = [&](float &data){
    //     data+=i;
    // };
    // auto hate_float = std::make_shared<test_thread::Hate<float>>("float", float_pro, 500);

    // while(true){
    
        
    //     auto float_data = hate_float->getdata();
    //     auto int_data = hate_int->getdata();
    //     std::cout<<i<<"  "<<int_data<<" "<<float_data<<std::endl;
    //     i++;
    //     if(i>10) break;
    // }

    // auto test5 = std::make_shared<Funcpt>(2);
    // std::cout<<"init data "<<test5->inter_data_->a_copy<<std::endl;
    // test5.testcall(f11, 2);
    // test5.testcall(Funcpt::inf_t, 2);
    // test5->testcall2();


    // std::uint32_t data1 = 255;
    // std::uint32_t data2 = 256;
    // std::uint32_t data3 = 257;
    // std::uint32_t data4 = 511;
    // std::uint32_t data5 = 512;
    
    // std::cout<<(data1>>8)<<" "<<(data2>>8)<<" "<<(data3>>8)<<" "<<(data4>>8)<<" "<<(data5>>8)<<std::endl;

    // std::uint64_t data6 = 0b111101010101;
    // std::uint64_t data9 = 0b10101;
    // std::uint64_t data7 = (1<<6)-(1<<0);
    // auto data8 = ((data6 &(data7))>>0);
    // std::cout<<data6<<" "<<data8<<std::endl;
    // std::cout<<data9<<std::endl;
    // auto test6 = std::make_shared<ThreadjoinTest>();
    // std::cout<<"start init"<<std::endl;
    // test6->init();
    // std::cout<<"finish init"<<std::endl;

    // test6->run();
    // Cantrans test7;
    // test7.data = 0;
    // test7.buf[0] = 0xa0;
    // std::cout<<"test data is "<<test7.data<<std::endl;
    // std::cout<<std::hex<<" hex is "<<test7.data<<std::endl;
    // auto *test_data = new std::uint8_t [16]();
    // memset(test_data,0,16*sizeof(std::uint8_t));
    // test_data[0] = 0x10;
    // test_data[1] = 0x01;
    // test_data[2] = 0xaa;
    // test_data[3] = 0xbb;
    // test_data[4] = 0xcc;
    // test_data[5] = 0xdd;

    // auto test8 = std::make_shared<BinaryFileWrite>("/home/joyson/Desktop/workspace");
    // test8->chwrite(test_data, 8);
    // test8->lenWrite(test_data,8);
    // delete []test_data;
//     Cantrans input;
//     std::vector<std::uint8_t>candata;
//     input.data = 0x6d62ac6bbd111001;
//     for(int i = 0; i < 8; i++){
//         candata.emplace_back(input.buf[i]);
//     }
//     for(int i=0;i<8;i++){
//          std::cout<<std::hex<<std::setw(2)<<std::setfill('0')<<(int)candata[i]<<" ";
//     }
//     std::cout<<std::defaultfloat<<std::endl;

//     auto raw_u = Canparse::unPackSignal<std::uint32_t>(24, 14, candata);

//     auto raw_f = Canparse::unPackSignal<float>(24, 14, candata, 0, 0.01);

//     auto sign = Canparse::unPackSignal<std::uint8_t>(28, 1, candata);

//     float reserve =(sign == 1)? -1.0: 1.0;

//     auto yaw_rate = Canparse::unPackSignal<float>(24, 14, candata, 0, 0.01 * M_PI/180.0* reserve);
   
//    std::cout<<" raw u/f sign yaw_rate "<<raw_u<<" "<<raw_f<<" "<<(uint32_t)sign<<" "<<yaw_rate<<std::endl;
// std::vector<std::uint8_t>outdata(8,0);
// Canparse::packSignal(-1.95425, 24, 14, outdata, 0, 0.01*M_PI/180*-1.0);
// Canparse::packSignal(1, 38, 1, outdata);
// for(int i = 0; i<8;i++){
//     std::cout<<std::hex<<std::setw(2)<<std::setfill('0')<<(int)outdata[i]<<" ";
// }
// std::cout<<std::defaultfloat<<std::endl;
   
//    std::vector<std::uint8_t> test_data(8, 0);
//    Canparse::packSignal(6.462, 0, 15, test_data, 0, 0.075/3.6);
//    Canparse::packSignal(1, 15, 1, test_data);
//    Canparse::packSignal(-6.434, 16, 15, test_data, 0, 0.075/3.6*-1.0);
//    Canparse::packSignal(0, 31, 1, test_data);
//    Canparse::packSignal(6.434, 32, 16, test_data, 0, 0.075/3.6);
//    Canparse::packSignal(6.462, 48, 16, test_data, 0, 0.075/3.6);
//     for(int i = 0; i<8;i++){
//     std::cout<<std::hex<<std::setw(2)<<std::setfill('0')<<(int)test_data[i]<<" ";
//     }
//     std::cout<<std::dec<<std::defaultfloat<<std::endl;
//     float fl,fr,rl,rr;
//     auto fl_sign = Canparse::unPackSignal<uint8_t>(15,1, test_data);
//     auto fr_sign = Canparse::unPackSignal<uint8_t>(31,1, test_data);
//     float fl_reserve = (fl_sign == 1)? -1.0 : 1.0;
//     float fr_reserve = (fr_sign == 1)? -1.0 : 1.0;
//     fl = Canparse::unPackSignal<float>(0, 15, test_data, 0, 0.075/3.6*fl_reserve);
//     auto fr_test = Canparse::unPackSignal<float>(16, 16, test_data, 0, 0.075/3.6);
//     fr = Canparse::unPackSignal<float>(16, 15, test_data, 0, 0.075/3.6*fr_reserve);
//     rl = Canparse::unPackSignal<float>(32, 16, test_data, 0, 0.075/3.6);
//     rr = Canparse::unPackSignal<float>(48, 16, test_data, 0, 0.075/3.6);
//     std::cout<<" fl fr rl rr fr_test"<<fl<<" "<<fr<<" "<<rl<<" "<<rr<<" "<<fr_test<<std::endl;

// auto test_fif = std::make_shared<ReadHexfile>("/home/joyson/docker_data/oem7_headingb.txt");
// test_fif->getbuf();
// std::uint64_t before = std::chrono::system_clock::now().time_since_epoch().count();
// usleep(500000);
// std::uint64_t after = std::chrono::system_clock::now().time_since_epoch().count();
// std::cout<<before<<std::endl;
// std::cout<<after<<std::endl;
// std::uint8_t rawdata[8] = {0,0,0xe0, 0xa0, 0,0,0,0};
// int rl = ((int)rawdata[2] >> 4) + ((((int)rawdata[3])& ((1<<5)-1)) << 4);
// int rr = (((int)rawdata[3]) >> 6)+(((int)rawdata[4]) << 2);

// std::cout<<"rear left/right "<<rl<<" "<<rr<<std::endl;

// std::vector<std::uint8_t>data(8, 0U);
// float acc , outputacc;
// std::cin>>acc;
// CanTrans::packSignal(acc, 16, 11, data, -7.22f, 0.005, true);
// std::cout<<"origin hex data "<<std::hex<<std::setw(2)<<std::setfill('0')<<(int)data[2]<<" "<<(int)data[3]<<std::endl;
// outputacc = CanTrans::unPackSignal<float>(16, 11, data, -7.22f, 0.005, true);
// std::cout<<"ouput signal "<<std::dec<<std::defaultfloat<<outputacc<<std::endl;

// uint8_t raw_data[1500] = {0};
// std::vector<uint8_t> data_vec;
// std::vector<uint8_t> data_v;
// auto ass_start = std::chrono::steady_clock::now();
// data_vec.assign(&raw_data[0], &raw_data[499]);
// auto middle = std::chrono::steady_clock::now();
// for(int i =0; i<1500;i++){
//    data_v.emplace_back(0);
// }
// auto end_time = std::chrono::steady_clock::now();

// auto assign_cost = std::chrono::duration_cast<std::chrono::microseconds>(middle - ass_start).count();
// auto empalce_cost = std::chrono::duration_cast<std::chrono::microseconds>(end_time - middle).count();
// printf("empalce %ld - assign %ld= %ld\n", empalce_cost, assign_cost, empalce_cost-assign_cost);

// int pdata[8]={0,1,2,3,4,5,6,7};

// int pp = SevenTeenTest::unPack<int>(6, 8,pdata);
// std::cout<<"pp "<<pp<<std::endl;

// SevenTeenTest::Signeddata test_signed;
// test_signed.buffer[0] = 0x09;
// test_signed.buffer[1] = 0x80;
// std::cout<<std::dec<<test_signed.s_data<<" "<<test_signed.u_data<<std::endl;

// SevenTeenTest::Transcan test17_1, test17_2;
// std::cout<<" test init: u_data"<<std::dec<<test17_1.unsigned_data<<" "<<std::hex<<test17_1.unsigned_data<<std::endl;
// std::cout<<"test init u_8 arrary :";
// for(int i = 0; i<8;i++ ) 
//     std::cout<<" "<<std::hex<<std::setw(2)<<std::setfill('0')<<(int)test17_1.buffer[i];
// std::cout<<std::endl;
// std::cout<<std::defaultfloat<<"test init bitset"<<test17_1.bit_set<<std::endl;


// test17_1.unsigned_data = 519;
// std::cout<<" test1: u_data"<<std::dec<<test17_1.unsigned_data<<" "<<std::hex<<test17_1.unsigned_data<<std::endl;
// std::cout<<"test1 u_8 arrary :";
// for(int i = 0; i<8;i++ ) 
//     std::cout<<" "<<std::hex<<std::setw(2)<<std::setfill('0')<<(int)test17_1.buffer[i];
// std::cout<<std::endl;
// std::cout<<std::defaultfloat<<"test1 bitset"<<test17_1.bit_set<<std::endl;


// test17_1.buffer[0] = 7;
// test17_1.buffer[1] = 2;
// std::cout<<" test2: u_data"<<std::dec<<test17_1.unsigned_data<<" "<<std::hex<<test17_1.unsigned_data<<std::endl;
// std::cout<<"test2 u_8 arrary :";
// for(int i = 0; i<8;i++ ) 
//     std::cout<<" "<<std::hex<<std::setw(2)<<std::setfill('0')<<(int)test17_1.buffer[i];
// std::cout<<std::endl;
// std::cout<<std::defaultfloat<<"test2 bitset"<<test17_1.bit_set<<std::endl;

// test17_2.bit_set.set(9);test17_2.bit_set.set(2);
// test17_2.bit_set.set(1);test17_2.bit_set.set(0);
// std::cout<<" test3: u_data"<<std::dec<<test17_2.unsigned_data<<" "<<std::hex<<test17_2.unsigned_data<<std::endl;
// std::cout<<"test3 u_8 arrary :";
// for(int i = 0; i<8;i++ ) 
//     std::cout<<" "<<std::hex<<std::setw(2)<<std::setfill('0')<<(int)test17_2.buffer[i];
// std::cout<<std::endl;
// std::cout<<std::defaultfloat<<"test3 bitset"<<test17_2.bit_set<<std::endl;

// std::vector<std::uint8_t> test_data;
// test_data.assign(8, 0);

// CanTransBit dest_data;
// float veh_yawrate, veh_velocity, pcan_yawrate, pcan_velocity;
// packSignal(3.15f, 40, 16, test_data, 0, 0.01/3.6);
// packSignal(0.0125, 24, 14, test_data, 0, 0.01*M_PI/180.0);

// auto veh_start = std::chrono::steady_clock::now();
// for(int k =0; k<50000;k++){
//     veh_yawrate = unPackSignal<float>(24, 14, test_data, 0, 0.01*M_PI/180.0);
//     veh_velocity =  unPackSignal<float>(40, 16, test_data, 0, 0.01/3.6);
// }
// auto middle = std::chrono::steady_clock::now();

// for(int k = 0; k<50000;k++){
// for(int i = 0;i<8;i++){
//     dest_data.buffer[i] = test_data[i];
// }
// pcan_yawrate = unPackSignalBitset<float>(24, 14, dest_data.bit_set, 0, 0.01*M_PI/180.0);
// pcan_velocity =  unPackSignalBitset<float>(40, 16, dest_data.bit_set, 0, 0.01/3.6);
// }

// auto pcan_end = std::chrono::steady_clock::now();

// uint64_t veh_use = std::chrono::duration_cast<std::chrono::microseconds>(middle - veh_start).count();
// uint64_t pcan_use = std::chrono::duration_cast<std::chrono::microseconds>(pcan_end - middle).count();

// printf("yawrate veh : %.3f  # pcan : %.3f\n", veh_yawrate,pcan_yawrate);
// printf("velocity veh : %.3f # pcan : %.3f\n",veh_velocity, pcan_velocity);
// printf("veh use: %lu  # pcan use : %lu  #diff %ld\n",veh_use, pcan_use, (int64_t)(veh_use-pcan_use));


// Eigen::Vector3d input = Eigen::Vector3d(3, 1, 0);
// Eigen::Vector3d euler =  Eigen::Vector3d(0., 0., M_PI_2);
// Eigen::Quaterniond q = eulerAngle2Quaternion(euler);
// auto result = q.conjugate() * input;
// std::cout<<"result"<<std::endl;
// std::cout<<result<<std::endl;
// std::cout<<"--------------"<<std::endl;
// std::cout<<q.conjugate()<<std::endl;
// std::cout<<q.w()<<std::endl;

// EighteenTest::MemcpyMat mcpy_test;

// mcpy_test.test(6);

/*NighteenTest::SignDataParse parse_test;
std::uint8_t input[8] = {0};*/
/* input[0] = 0b00000111;input[1] = 0b10011\
001;input[2] = 0b11\
000101;input[3] = 0b00110010;
input[4] = 0b00\
000100;input[5] = 0b10001010;input[6] = 0b0\
0000011;input[7] = 0b00111100;*/
/*input[0] = 0b11111000;input[1] = 0b01100\ 
110;input[2] = 0b11\
111010;input[3] = 0b11001101;
input[4] = 0b11\
111011;input[5] = 0b01110101;input[6] = 0b0\
1111100;input[7] = 0b00111100;

parse_test.parse_candata(&input[0]);*/

// SwitchTesT switch_test;
// switch_test.Test(1);
// switch_test.Test(999);
// switch_test.Test(4);
// switch_test.Test(2);
// switch_test.Test(5);
// switch_test.Test(999);
// switch_test.Test(8);


// GflagsTest flagtest;
// flagtest.init(argc, argv);
// flagtest.test();

// Eigen::Quaterniond q(0.578090,-0.024558,0.011323,0.815525);
// Eigen::Vector3d euler = rotation::quaternion2EeulerAngle(q);
// euler = 180.f/M_PI * euler;
// std::cout<<euler<<std::endl;

// Eigen::Vector3d in_euler(0.f, 0.f, 0.126597);
// in_euler = M_PI/180.f *  in_euler;
// auto q = rotation::eulerAngle2Quaternion(in_euler);
// printf("%.7f,%.7f,%.7f,%.7f\n",q.w(),q.x(),q.y(),q.z());


// Eigen::Vector3d acc_m (-0.245395410, 0.410674895, 9.780088341);
// Eigen::Vector3d g(0., 0., -9.79990);
// auto ba = q.conjugate() * g + acc_m;
// auto ba2 = q * g +acc_m;
// std::cout<<"ba:"<<std::endl;
// std::cout<<ba.transpose()<<std::endl;
// std::cout<<"ba2:"<<std::endl;
// std::cout<<ba2.transpose()<<std::endl;
// Eigen::Vector3d qv(0.0000110,0.0001704,-0.0002541);
// Eigen::Vector3d input_eler(2.65965,6.33549,-151.62099);
// input_eler *=M_PI/180.f;
// auto qd = rotation::eulerAngle2Quaternion(input_eler);
// auto out_q = qd * rotation::angleAxis2Quaternion(qv);
// Eigen::Vector3d output_eler = rotation::quaternion2EeulerAngle(out_q);
// output_eler *= 180.f/M_PI;
// std::cout<<output_eler<<std::endl;


// normtest::SonclassA a(normtest::Baseclass::static_a);
// normtest::SonclassB b;

// b.set_a(9);
// a.test();
// b.test();

// a.set_a(5);
// a.static_a ++;
// a.test();
// normtest::Baseclass::static_a ++;
// b.test();

// normtest::SonclassB::static_a ++;
// b.test();
// b.static_a = 111;
// a.test();

// Eigen::Vector3d delta_e = Eigen::Vector3d::Zero();
// delta_e.z() = M_PI / 3.f;
// maniftest::ManifTest update_test(delta_e);
// update_test.test();

// std::string true_file("/home/joyson/docker_data/eskf_debug/truth.csv");
// imu_test::Predictor  pred_test(true_file);
// pred_test.test();
// Eigen::Vector3d asix(-0.0001620,-0.00012400046,-1563.00010000010);
// Eigen::Quaterniond q(manif::exp(manif::SO3Tangentd(asix)).rotation());
// auto euler = rotation::quaternion2EeulerAngle(q);
// euler =180.f/M_PI *euler;
// std::cout<<euler<<std::endl;

// for(int i=0;i<50;i++){
//     auto pt = Eigen::VectorXd::Random(1)(0);
//     std::cout<<pt<<std::endl;
//     usleep(10000);
// }
// quetest::EndTest end_test;
// end_test.test();

// DequeTest deque_test;
// deque_test.test();
// Eigen::Vector4d output;
// Eigen::Vector2d inp1 = Eigen::Vector2d(1., -1.);
// Eigen::Vector2d inp2 = Eigen::Vector2d(9., -9.);
// output<<inp1,inp2;

// std::cout<<output.transpose()<<std::endl;

// typedef struct RbgData{
//     int id;
//     uint16_t r;
//     uint16_t b;
//     RbgData(const int i, const uint16_t red,  const uint16_t blue){
//         this->id = i;
//         this->r = red;
//         this->b = blue;
//     }

// };
// std::deque<std::shared_ptr<RbgData>> testdque;
// testdque.push_back(std::make_shared<RbgData>(1, 255, 0));
// if(testdque.size()>2){
//     testdque.erase(testdque.end()-2);
// }
// for(size_t i=0; i<testdque.size();i++){
//     printf("%d  ", testdque[i]->id);
// }
// printf("\n-------------\n");

// auto inp_data = std::make_shared<RbgData>(2, 122, 0);
// testdque.push_back(inp_data);
// inp_data = std::make_shared<RbgData>(3, 0,122);
// testdque.push_back(inp_data);
// if(testdque.size()>2){
//     testdque.erase(testdque.end()-2);
// }
// for(size_t i=0; i<testdque.size();i++){
//     printf("%d  ", testdque[i]->id);
// }
// printf("\n-------------\n");

// inp_data = std::make_shared<RbgData>(4, 0,255);
// testdque.push_back(std::make_shared<RbgData>(*inp_data));
// auto inp_data2 = std::make_shared<RbgData>(5, 255, 255);
// testdque.push_back(inp_data2);

// if(testdque.size()>2){
//     testdque.erase(testdque.end()-2);
// }
// for(size_t i=0; i<testdque.size();i++){
//     printf("%d  ", testdque[i]->id);
// }
// printf("\n-------------\n");

// if(testdque.size()>2){
//     testdque.erase(testdque.end()-2);
// }
// for(size_t i=0; i<testdque.size();i++){
//     printf("%d  ", testdque[i]->id);
// }
// printf("\n-------------\n");


// testdque.push_back(std::make_shared<RbgData>(6, 255, 0));
// testdque.erase(testdque.end()-2);
// for(size_t i=0; i<testdque.size();i++){
//     printf("%d  ", testdque[i]->id);
// }

// testdque.push_back(std::make_shared<RbgData>(7, 255, 0));
// testdque.push_back(std::make_shared<RbgData>(8, 255, 0));
// printf("\n-------------\n");
// testdque.erase(testdque.end()-2);
// for(size_t i=0; i<testdque.size();i++){
//     printf("%d  ", testdque[i]->id);
// }

// JMT rrtest;
// rrtest.test();

// stringtest::Stringcut str_test("/.././docker_data/topic_jsslam/dataset/2023_10_24_14-13-15.bag");
// str_test.test();

// double stamp_a = 0.;

// double stamp_b = std::numeric_limits<float>::min();

// std::cout<<"float min "<<stamp_b<<std::endl;
 
// double tmp = 0.;

// int compare = (stamp_a == (tmp + stamp_b)? 0 :(stamp_a < (tmp + stamp_b)?1:-1));

// std::cout<<"compare stamp a b "<<compare<<std::endl;




// auto euler = Eigen::Vector3d(15., 27., 90.);
// euler = M_PI/180. * euler;

// auto gyro = Eigen::Vector3d(0., 45., 15.);
// gyro = M_PI/180. * gyro;

// auto d_add = euler + gyro *2.;
// std::cout<<"direct add "<<(180./M_PI * d_add).transpose()<<std::endl;

// auto b_anglex = jsos::utility::angleAxis2Quaternion<double>(gyro *2.) * jsos::utility::eulerAngle2Quaternion(euler);
// auto b_euler = jsos::utility::quaternion2EeulerAngle(b_anglex);
// std::cout<<"b add "<<(180./M_PI * b_euler).transpose()<<std::endl;
// // auto after = jsos::utility::angleAxis2Quaternion(Eigen::Vector3d(-0.000157085,0.0000449435,0.00405854)) * jsos::utility::eulerAngle2Quaternion(euler);
// // auto after_euler = jsos::utility::quaternion2EeulerAngle(after);

// auto e_anglex = jsos::utility::eulerAngle2Quaternion(euler) * jsos::utility::angleAxis2Quaternion<double>(gyro *2.);
// auto e_euler = jsos::utility::quaternion2EeulerAngle(e_anglex);
// std::cout<<"e add "<<(180./M_PI * e_euler).transpose()<<std::endl;

// auto manif_rot = jsos::utility::eulerAngle2Quaternion(euler) * manif::exp(manif::SO3Tangentd(gyro *2.)).rotation();
// auto manif_euler = jsos::utility::quaternion2EeulerAngle(Eigen::Quaterniond(manif_rot));
// std::cout<<"manif add "<<(180./M_PI * manif_euler).transpose()<<std::endl;

// Eigen::Matrix<double,3,3> R1, R2, R3;
// R1 << 1.,   0.,     0., 
//       0.,   1.,    0.,
//       0.,   0.,    1.;
// R2 << 1.,    0.,      0.,
//       0.,            1.,      0.,
//       0.,   0.,     1.;

// R3 << cos(M_PI/3.),  sin(M_PI/3.),    0.,
//       -1.*sin(M_PI/3.),   cos(M_PI/3.),       0.,
//       0.,             0.,                 1.;
// auto right_matrix_euler = R1*R2*R3*euler;
// std::cout<<"matrix right add "<<(180./M_PI * right_matrix_euler).transpose()<<std::endl;

// auto left_matrix_euler = R3*R2*R1*euler;
// std::cout<<"matrix left add "<<(180./M_PI * left_matrix_euler).transpose()<<std::endl;


// Eigen::Vector3d acce_mean(0.5, sqrt(3)/2., 0);
// Eigen::Vector3d gravity (0., 0., 1.);
// auto tmp_q = Eigen::Quaterniond::FromTwoVectors(acce_mean.normalized(), gravity.normalized());
// auto euler = jsos::utility::quaternion2EeulerAngle(tmp_q);
// euler = 180./M_PI * euler;
// std::cout<<"res1: "<<euler.transpose()<<std::endl;
// auto acce_b = Eigen::Vector3d(0.1, sqrt(3)/2., 0);
// auto tmp_qb = Eigen::Quaterniond::FromTwoVectors(acce_b.normalized(), gravity.normalized());
// euler = jsos::utility::quaternion2EeulerAngle(tmp_qb);
// euler = 180./M_PI * euler;
// std::cout<<"res2: "<<euler.transpose()<<std::endl;


// Eigen::Vector3d euler(0., 0., 90.);
// Eigen::Vector3d omega(300., 900., 10.);
// euler =M_PI/180. * euler; omega = M_PI/180. * omega;
// double delat_time = 0.1;
// Eigen::Vector3d delta_theta = delat_time * omega;
// Eigen::Quaterniond start_pose = jsos::utility::eulerAngle2Quaternion(euler);
// auto jsos_q = start_pose * jsos::utility::angleAxis2Quaternion(delta_theta) ;
// auto jsos_e = jsos::utility::quaternion2EeulerAngle(jsos_q);
// std::cout<<"jsos angle:  " <<(180./M_PI * jsos_e).transpose()<<std::endl;

// double sita0 = omega.norm() * delat_time;
// Eigen::Matrix<double, 4, 1> ztia;

// ztia(0) = cos(euler.z()/2.0)*cos(euler.x()/2.0)*cos(euler.y()/2.0)-sin(euler.z()/2.0)*sin(euler.x()/2.0)*sin(euler.y()/2.0);
// ztia(1) = cos(euler.z()/2.0)*sin(euler.x()/2.0)*cos(euler.y()/2.0)-sin(euler.z()/2.0)*cos(euler.x()/2.0)*sin(euler.y()/2.0);
// ztia(2) = cos(euler.z()/2.0)*cos(euler.x()/2.0)*sin(euler.y()/2.0)+sin(euler.z()/2.0)*sin(euler.x()/2.0)*cos(euler.y()/2.0);
// ztia(3) = cos(euler.z()/2.0)*sin(euler.x()/2.0)*sin(euler.y()/2.0)+sin(euler.z()/2.0)*cos(euler.x()/2.0)*cos(euler.y()/2.0);

// Eigen::Matrix<double, 4, 4> sita;
// sita << 0.,             -1. * delta_theta.x(),  -1. * delta_theta.y(),  -1. * delta_theta.z(),
//         delta_theta.x(), 0.,                    delta_theta.z(),        -1. * delta_theta.y(),
//         delta_theta.y(),-1. * delta_theta.z(),  0.,                     delta_theta.x(),
//         delta_theta.z(),delta_theta.y(),        -1. * delta_theta.x(),  0.;

// ztia = (cos(sita0/2.)*Eigen::Matrix4d::Identity() + sin(sita0/2.)/sita0 * sita) * ztia;
// ztia.normalize();
// Eigen::Quaterniond sinout_q;
// sinout_q.w() = ztia.x(); sinout_q.x() = ztia.y();sinout_q.y() = ztia.z(); sinout_q.z() = ztia.w();
// auto sinout_e = jsos::utility::quaternion2EeulerAngle(sinout_q);
// std::cout<<" sinout angle: "<<(180./M_PI * sinout_e).transpose()<<std::endl;
// Eigen::Vector3d euler = Eigen::Vector3d(0., 0., 1.8);
// euler = M_PI/180. * euler;
// Eigen::Quaterniond rot = jsos::utility::eulerAngle2Quaternion(euler);
// double  degree = 2. * std::acos(rot.w()) *180./M_PI;
// printf("[%.7f %.7f %.7f %.7f]   %.7f\n", rot.w(), rot.x(), rot.y(), rot.z(), degree);

// InsertTest insert_test;
// insert_test.test(6,16);
// insert_test.test3();

// PtrTest ptr_test;
// ptr_test.test();

// VelRotation vel_rot_test;
// vel_rot_test.test();

// enum Action{
//     Eat = 0,
//     Sleep = 1,
//     Die = 2,
// };

// Action P1 = Action::Sleep;

// int a = !!(P1 == Action::Sleep);
// int b = !(P1 == Action::Sleep);
// int c = !!(P1 == Action::Die);
// printf("%d   %d   %d\n", a, b, c);


// caculateFH::FHTest fh_test;
// fh_test.FTest();
// fh_test.HTset();
// threadtest::RestartThread restart_test;
// restart_test.test();


// strsstream::Sstm sstest;
// sstest.test();


// stleigen::StlEigen stl_eigen_test;
// stl_eigen_test.test();

// bindtest::BindTest b_test;
// b_test.test2(66);

using Msg1 = tamplate_func::Imu;
using Msg2 = tamplate_func::Image;
using Msg3 = std::shared_ptr<tamplate_func::Imu>;
using Msg4 = tamplate_func::Wheel;
tamplate_func::MutableSynchronizer<Msg1, Msg2, Msg3, Msg4> sync_test;
sync_test.setMsgTimestampFunction([&](const std::shared_ptr<Msg1> msg1)->std::uint64_t{return msg1->stamp;},
                                  [&](const std::shared_ptr<Msg2> msg2)->std::uint64_t{return msg2->header.stamp;},
                                  [&](const std::shared_ptr<Msg3> msg3)->std::uint64_t{return (*msg3)->stamp;},
                                  [&](const std::shared_ptr<Msg4> msg4)->std::uint64_t{return msg4->pair_head.second;});

auto msg1_data1 = std::make_shared<Msg1>(11U);
auto msg2_data1 = std::make_shared<Msg2>(22U);
auto msg3_data1 = std::make_shared<Msg3>(std::make_shared<Msg1>(31U));
auto msg4_data1 = std::make_shared<Msg4>(44U);
sync_test.insert<0>(msg1_data1);
sync_test.insert<1>(msg2_data1);
sync_test.insert<2>(msg3_data1);
sync_test.insert<3>(msg4_data1);
sync_test.test();
return 0;

}





