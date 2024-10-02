#include <iostream>
#include <string>

#include "friend.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"
#include <vector>
#include "logger.h"



class FriendService:public fixbug::FriendServiceRpc
{
public:
    std::vector<std::uint32_t> GetFriendsList(const uint32_t &uid)
    {
        std::cout << "UserService : GetFriendsList() is using,uid : " << uid << std::endl;
        std::vector<std::uint32_t> vec = {1001,1002,1003};
        return vec;
    }

     //重寫FriendServiceRpc中的GetFriendsList，藉助MPRPC框架將本地GetFriendsList發佈稱
    void GetFriendsList(::google::protobuf::RpcController* controller,
                       const ::fixbug::GetFriendsListRequest* request,
                       ::fixbug::GetFriendsListResponse* response,
                       ::google::protobuf::Closure* done)
    {
        uint32_t uid = request->uid();

        std::vector<std::uint32_t> vec = GetFriendsList(uid);

        for(uint32_t i : vec)
        {
            response->add_friends(i);
        }
        done->Run();
    }

};

int main(int argc,char **argv)
{

    LOG_INFO("first log message\n");
    LOG_ERR("%s:%s:%d\n", __FILE__, __FUNCTION__, __LINE__);

    // 调用框架的初始化方法
    MprpcApplication::init(argc, argv);

    // provider 是一个 rpc 网络服务对象，负责把服务对象发布到rpc节点上
    RpcProvider provider;
    provider.NotifyService(new FriendService());

    provider.Run();
    

}