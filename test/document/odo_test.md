# odo_test

## 模型假设：
   1. 忽略车轮侧滑角，后轮轮速为车速（不打滑）
   2. 两帧地盘车速数据时间内车速匀速变化
   3. 两帧imu航向角数据时间内yaw_rate不变

## 变量含义：
$boot\_coordinates\_[3]= [x, y, yaw]$ 开机坐标系  
$last\_time\_$    上一帧车速的时间戳  
$veh\_timestamp\_$  当前帧车速的时间戳  
$last\_spd\_val\_$ 上一帧后轮平均速度   
$spd\_val\_$   当前帧后轮平均速度  
$yaw\_rate\_$  imu输出的航向角速度  
$is\_first\_$   标志位  

## 运动公式：
$$ 
\Delta x = \int_0^{\Delta t} v*cos(\alpha_{0} + \omega* t) dt\\  
 = \frac{v}{\omega}(\sin(\alpha_0+\omega * \Delta t)- \sin(\alpha_0))
$$

$$ 
\Delta y = \int^{\Delta t}_0 v*sin (\alpha_0 + \omega*  t) dt\\  
 =  \frac{v}{\omega}( \cos(\alpha_0) -  \cos(\alpha_0+\omega * \Delta t))
$$

$$
\Delta \alpha = \int^{\Delta t}_0 \omega dt\\
    =\omega * \Delta t
$$



## 代码：
```
void odo_test(){

double boot_coordinates_[3];//开机坐标系 x y yaw
std::uint64_t last_time_ , //上一帧车速的时间戳
                veh_timestamp_;//当前帧车速的时间戳
float last_spd_val_,//上一帧后轮平均速度
        spd_val_;//当前帧后轮平均速度
double yaw_rate_;//imu输出的航向角速度
bool is_first_=true;//标志位




if(is_first_){
        boot_coordinates_[0] = 0;
        boot_coordinates_[1] = 0;
        boot_coordinates_[2] = 0;
        last_time_ = veh_timestamp_;
        last_spd_val_ = spd_val_;
        is_first_ = false;
    }
    else{
        double dt = (veh_timestamp_ - last_time_) * 1e-9;
        boot_coordinates_[2] += yaw_rate_ * dt;
        if(fabs(yaw_rate_) < 1e-3){
            boot_coordinates_[0] += (last_spd_val_ + spd_val_)/2.0 * cos(boot_coordinates_[2])*dt;
            boot_coordinates_[1] += (last_spd_val_ + spd_val_)/2.0 * sin(boot_coordinates_[2])*dt;
        }
        else{
            boot_coordinates_[0] += (last_spd_val_ + spd_val_)/2.0/ yaw_rate_ * (sin(boot_coordinates_[2])- sin(boot_coordinates_[2]- yaw_rate_ * dt));
            boot_coordinates_[1] += (last_spd_val_ + spd_val_)/2.0/ yaw_rate_ * (cos(boot_coordinates_[2]- yaw_rate_ * dt)- cos(boot_coordinates_[2]));
        }

    }
    last_time_ = veh_timestamp_;
    last_spd_val_ = spd_val_;
}
```





