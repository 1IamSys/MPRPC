#include <iostream>
#include <string>

#include "user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"




class UserService:public fixbug::UserServiceRpc
{
public:
    bool Login(std::string name,std::string pwd)
    {
        std::cout << "UserService : login() is using" << std::endl;
        std::cout << "name:" << name << "  pwd:" << pwd <<std::endl;
        return false;
    } 
    uint32_t Register(std::string name,std::string pwd)
    {
        std::cout << "UserService : Register() is using" << std::endl;
        std::cout << "name:" << name << "  pwd:" << pwd <<std::endl;
        return 1001;
    } 

    void Login(::google::protobuf::RpcController* controller,
                const ::fixbug::LoginRequest* request,
                ::fixbug::LoginResponse* response,
                ::google::protobuf::Closure* done)
    {

        std::string name = request->name();
        std::string pwd = request->pwd();

        bool Login_result = Login(name,pwd);

        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(1);
        code->set_errmsg("hello");
        response->set_success(Login_result);

        done->Run();
    }

    //藉助MPRPC框架將本地方法Register（）發佈稱Rpc方法
    void Register(::google::protobuf::RpcController* controller,
                       const ::fixbug::RegisterRequest* request,
                       ::fixbug::RegisterResponse* response,
                       ::google::protobuf::Closure* done)
    {
        std::string name = request->name();
        std::string pwd = request->pwd();

        uint32_t ret = Register(name,pwd);

        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        response->set_uid(ret);

        done->Run();
    }

};

int main(int argc,char **argv)
{
    // 调用框架的初始化方法
    MprpcApplication::init(argc, argv);

    // provider 是一个 rpc 网络服务对象，负责把服务对象发布到rpc节点上
    RpcProvider provider;
    provider.NotifyService(new UserService());

    provider.Run();
    

}