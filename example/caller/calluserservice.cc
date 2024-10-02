#include <iostream>
#include "mprpcapplication.h"
#include "user.pb.h"



int main(int argc,char **argv)
{
    MprpcApplication::init(argc,argv);

    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    // stub.Login();

    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");

    fixbug::LoginResponse response;
    
    //发起RPC方法的调用 异步阻塞的调用过程
    stub.Login(nullptr,&request,&response,nullptr);



    if(0 == response.result().errcode()) //響應成功
    {
        std::cout << "rpc login response success,result: " << response.success() << std::endl;
    }
    else
    {
        std::cout << "rpc login response error,because: " << response.result().errmsg() << std::endl;
    }

    fixbug::RegisterRequest re_request;
    request.set_name("shishuai");
    request.set_pwd("123456");

    fixbug::RegisterResponse re_response;

    stub.Register(nullptr,&re_request,&re_response,nullptr);

    if(0 == re_response.result().errcode()) //響應成功
    {
        std::cout << "rpc register response success,result: " << re_response.uid() << std::endl;
    }
    else
    {
        std::cout << "rpc register response error,because: " << re_response.result().errmsg() << std::endl;
    }




    return 0;
}