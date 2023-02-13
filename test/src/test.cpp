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
#include <test5.h>
#include <test6.h>
#include <test7.h>
#include <test8.h>
#include <test9.h>
#include <random>
#include <thread>
#include <chrono>
#include <sstream>
#include <iomanip>



void fun(int &a, int &b){
    std::cout<<"a ,b ("<<a<<","<<b<<")"<<std::endl;
}

void f1( TestEi::DataA& data){
    auto a = data;
    a.run();
    std::cout<<"----------"<<std::endl;
}

void f2( TestEi::DataB& data){
    auto b = data;
    b.run();
    std::cout<<"==========="<<std::endl;
}

template<typename T>
void mycout (double a){
    T b = (T)a;
    std::cout<<b<<std::endl;
}

using namespace Test;

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
    // std::shared_ptr<std::vector<double>> test (new std::vector<double>(5,1.0));
    // auto clone_test = std::make_shared<std::vector<double>>(std::vector<double>(*test.get()));
    // for(int i = 0;i<test->size();i++){
    //     test->at(i) *= i;
    // }
    // for(int i = 0;i<test->size();i++){
    //     std::cout<<test->at(i)<<" ";
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

    // auto test = std::make_shared<Test::TestSev>();
    // test->register_recorder("imu");
    // test->register_recorder("raw");
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
    //       test->record("imu", format_a, t,data_a.a1, data_a.a2, data_a.a3, data_a.a4.c_str(), data_a.sts);
           
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
    //       test->record("imu", format_a, t,data_b.b1.c_str(), 
    //                     data_b.sts, data_b.b2, data_b.b3);
    //       if((data_a3/10)%2==0)  data_a2 *=-1.0;
    //       test->record("raw", format_raw, data_timestamp, t,
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

    
    // auto test = std::make_shared<TestEi::Sub>();
    // void (*data_Acallback) ( TestEi::DataA &data) = f1;
    // void (*data_Bcallback) ( TestEi::DataB &data) = f2;
    // void (*data_Bcallback)(const TestEi::DataB &data) = [&](const TestEi::DataB &data){
    //     std::cout<<"============="<<std::endl;
    // };
    // test->register_handler(data_Acallback);
    // test->register_handler(data_Bcallback);
    // test->run();
    // std::vector<std::string>test_coll;

    // std::string test ={"R: 345267171171 121.54682821 31.2517907 3.163 152.34 1.362 -0.466"};
    // test_coll.push_back(test);
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

    std::cout<<getTypename<Recorder>()<<std::endl;
    std::cout<<getTypename<TestEi::Sub::Listener>()<<std::endl;
    
    return 0;
}





