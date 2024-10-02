#include "mprpcchannel.h"
#include <string>
#include "rpcheader.pb.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include "mprpcapplication.h"


void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                          google::protobuf::RpcController* controller, const google::protobuf::Message* request,
                          google::protobuf::Message* response, google::protobuf::Closure* done)
    {
        const google::protobuf::ServiceDescriptor *sd  = method->service();

        std::string service_name = sd->name();
        std::string method_name = method->name();

        uint32_t args_size = 0;
        std::string args_str;
        if(request->SerializeToString(&args_str))
        {
            args_size = args_str.size();
        }
        else
        {
            controller->SetFailed("request Serialize error");
            return;
        }

        mprpc::RpcHeader rpcheader;
        rpcheader.set_service_name(service_name);
        rpcheader.set_method_name(method_name);
        rpcheader.set_agrs_size(args_size);
        uint32_t header_size = 0;
        std::string rpc_header_str;
        if(rpcheader.SerializeToString(&rpc_header_str))
        {
            header_size = rpc_header_str.size();
        }
        else
        {
            controller->SetFailed("rpc_header_str Serialize erro");
            return;
        }


        // 組織帶發送的rpc請求的字符串
        std::string send_rpc_str;

        send_rpc_str.insert(0,std::string((char*)&header_size,4)) ;//(前四個字節放請求頭的大小)

        send_rpc_str += rpc_header_str; // 數據頭
        send_rpc_str += args_str;  //參數


        //打印調試信息
        std::cout << "===========================================" << std::endl;
        std::cout << "header_size : " << header_size << std::endl;
        std::cout << "rpc_header_str : " << rpc_header_str << std::endl;
        std::cout << "service_name : " << service_name << std::endl;
        std::cout << "method_name : " << method_name << std::endl;
        std::cout << "args_str : " << args_str << std::endl;
        std::cout << "===========================================" << std::endl;


        //使用tcp編程 完成rpc方法的遠程調用
        int clientfd = socket(AF_INET,SOCK_STREAM,0);

        char str[1100];//存儲錯誤信息

        if(-1 == clientfd)
        {
            sprintf(str,"create socket error! errno:%d",errno);
            controller->SetFailed(str);
            return;
        }

        std::string ip = MprpcApplication::GetInstance().GetMyConfig().Load("rpcserverip");
        uint16_t port = atoi(MprpcApplication::GetInstance().GetInstance().GetMyConfig().Load("rpcserverport").c_str());

        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

        //連接rpc服務節點
        if(-1 == (connect(clientfd,(struct sockaddr*)&server_addr,sizeof(server_addr))))
        {
            sprintf(str,"connect error! errno:%d",errno);
            controller->SetFailed(str);
            close(clientfd);
            return;
        }

        //發送請求
        if(-1 == send(clientfd,send_rpc_str.c_str(),send_rpc_str.size(),0))
        {
            sprintf(str,"send error! errno:%d",errno);
            controller->SetFailed(str);
            close(clientfd);
            return ;
        }

        //接受響應

        char recv_buf[1024] = {0};
        int recv_size;
        if(-1 == (recv_size = recv(clientfd,recv_buf,1024,0)))
        {
            sprintf(str,"receive error! errno:%d",errno);
            controller->SetFailed(str);
            close(clientfd);
            return ;
        }

        //反序列化：解析數據到response
        // std::string response_str(recv_buf,0,recv_size); //有bug
        // if(!response->ParseFromString(response_str))
        if(!response->ParseFromArray(recv_buf,recv_size))
        {
            sprintf(str,"parse error! rec_buf:%s",recv_buf);
            controller->SetFailed(str);
            close(clientfd);
            return ;
        }
        close(clientfd);
        
    }