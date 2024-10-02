#include <iostream>
#include "mprpcapplication.h"
#include "friend.pb.h"
#include "mprpcchannel.h"


int main(int argc,char **argv)
{
    MprpcApplication::init(argc,argv);

    fixbug::FriendServiceRpc_Stub stub(new MprpcChannel());
    // stub.Login();

    fixbug::GetFriendsListRequest request;

    request.set_uid(1001);

    fixbug::GetFriendsListResponse response;

    //控制信息(用來判斷框架在執行過程中是否出現錯誤)
    MprpcController mprpccontroller;
    //发起RPC方法的调用 异步阻塞的调用过程
    stub.GetFriendsList(&mprpccontroller,&request,&response,nullptr);

    if(mprpccontroller.Failed())
    {
        std::cout << mprpccontroller.ErrorText() <<std::endl;
    }
    else
    {
        if(0 == response.result().errcode()) //響應成功
        {
            int friend_size = response.friends_size();

            for(int i = 0;i<friend_size;++i)
            {
                std::cout << "rpc login response success;friend"  << i <<" :" << response.friends(i) << std::endl;
            }
            
        }
        else
        {
            std::cout << "rpc login response error,because: " << response.result().errmsg() << std::endl;
        }
    }

    

    return 0;
}