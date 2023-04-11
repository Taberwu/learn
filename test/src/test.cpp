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
#include <test16.hpp>
#include <random>
#include <thread>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <vector>



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

int main(int argc, char **argv){
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

uint8_t raw_data[500] = {0};
std::vector<uint8_t> data_vec;
std::vector<uint8_t> data_v;
auto ass_start = std::chrono::steady_clock::now();
data_vec.assign(&raw_data[0], &raw_data[499]);
auto middle = std::chrono::steady_clock::now();
for(int i =0; i<500;i++){
   data_v.emplace_back(0);
}
auto end = std::chrono::steady_clock::now();

auto assign_cost = std::chrono::duration_cast<std::chrono::milliseconds>(middle - ass_start).count();

 return 0;
}





