#include "modules/Node/include/load_node.h"

//=======config area start=========
#include "modules/dr/include/DomainLFNode.h"
//=======config area end=========

int main(int argc, char **argv)
{
    //=======config area start=========
    int64_t intervaltimetime=10;//ms
    std::string app_name="node1";
    std::string lfnode_name="dr";
    typedef typename atd::dr::DomainLFNode LFNodeType;
    //=======config area end=========

    std::shared_ptr<atd::node::GenericShellNode> gsnode=atd::node::GsNodeLoader::getObject(app_name);
    gsnode->Init();

    //=======config area start=========
    gsnode->SetTimeEnv(atd::node::TimeEnv::from_int64_t_micro_to_gen_second);
    gsnode->monitor_->ConfigSet(true,false);
    //=======config area end=========

    atd::node::NodeLoader<LFNodeType>::set_init_config(gsnode.get());
    auto lfnode=atd::node::NodeLoader<LFNodeType>::getObject(lfnode_name);
    lfnode->Init();

    //runtime
    atd::common::Timer _timer_;
    while(1)
    {
        _timer_.Start();

        //=======config area start=========
        RECV_LCM_TO_NODE(RAW_INS,RAW_INS,lfnode,RAW_INS,false);
        RECV_LCM_TO_NODE(INS300C,INS300C58,lfnode,INS300C,false);
        RECV_LCM_TO_NODE(VCU_VEHILCE_INFO,VCU_VEHICLE_INFO,lfnode,Vcu,false);
        //=======config area end=========

        atd::node::Status status_=lfnode->Step();
        if(!status_.ok())
        {
            gsnode->monitor_->FATAL(google::protobuf::StrCat
                                    ("[",gsnode->get_name(),"]->","[",lfnode->get_name(),"]->",status_.ToString()));
        }

        //=======config area start=========
        SEND_NODE_TO_LCM(lfnode,Dr,atd::common::pb_p_mutiarrow,Dr,false);
        //=======config area end=========


        gsnode->Step();

        MainLoopSleep(intervaltimetime,_timer_.End(),true);

    }

    return 0;
}





