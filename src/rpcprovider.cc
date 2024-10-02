#include "rpcprovider.h"
#include "mprpcapplication.h"
#include <functional>
#include "rpcheader.pb.h"
#include "logger.h"

//供框架外部使用的，可以发布 RPC 方法的函数接口
void RpcProvider::NotifyService(google::protobuf::Service *service)
{
    const google::protobuf::ServiceDescriptor *pserviceDesc = service->GetDescriptor();

    std::string service_name = pserviceDesc->name();

    int methodCnt = pserviceDesc->method_count();

    std::cout << "service_name : " << service_name << std::endl;

    ServiceInfo service_info;
    service_info.m_service = service;
    for(int i = 0;i<methodCnt;++i)
    {
        const google::protobuf::MethodDescriptor *pmethodDesc = pserviceDesc->method(i);// 获取方法描述符
        std::string method_name = pmethodDesc->name();
        service_info.m_methodMap.insert({method_name,pmethodDesc});
        std::cout << "method_name : " << method_name << std::endl;
    }
    
    m_serviceMap.insert({service_name,service_info});

}



// 启动 RPC 服务节点，开始提供 RPC 远程网络调用服务
void RpcProvider::Run()
{
    std::string ip = MprpcApplication::GetInstance().GetMyConfig().Load("rpcserverip");
    uint16_t port = atoi(MprpcApplication::GetInstance().GetMyConfig().Load("rpcserverport").c_str());
    muduo::net::InetAddress address(ip,port);

    // 创建 TcpServer 对象
    muduo::net::TcpServer server(&m_eventLoop, address, "RpcProvider");

    // 绑定连接回调和消息读写回调方法，分离了网络代码和业务代码
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection,this,std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage,this,std::placeholders::_1,
                               std::placeholders::_2,std::placeholders::_3));


    // 设置 muduo 库的线程数量
    server.setThreadNum(4);

    std::cout << "RpcProvider start at IP: " << ip << " port: " << port << std::endl;
    // 启动网络服务
    server.start();
    m_eventLoop.loop();

    
}


void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr& conn)
{
    if(!conn->connected())  // 客户端断开连接
    {
        conn->shutdown();
    }

}

void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr& conn,muduo::net::Buffer*buffer, muduo::Timestamp)
{
    // 4(字節) + 數據頭（服務名+方法名+參數大小）+ 實際參數
    std::string recv_buff = buffer->retrieveAllAsString();

    uint32_t header_size = 0;

    recv_buff.copy((char *)&header_size, 4, 0);  // 获取字符流的前4个字节信息，表示除参数外的数据头大小

    std::string rpc_header_str = recv_buff.substr(4, header_size); // 数据头（不带参数的）字符串

    // 解析数据头（不带参数的）字符流数据；
    mprpc::RpcHeader rpcheader;
    std::string service_name;
    std::string method_name;
    uint32_t args_size;
    if(rpcheader.ParseFromString(rpc_header_str))
    {
        service_name = rpcheader.service_name();
        method_name = rpcheader.method_name();
        args_size = rpcheader.agrs_size();
    }
    else
    {
        std::cout << "rpc_header_str" << rpc_header_str << "parse error" << std::endl;
        return ;
    }

    // 解析参数的字符流数据
    std::string args_str = recv_buff.substr(4 + header_size,args_size);

        //打印調試信息
    std::cout << "===========================================" << std::endl;
    std::cout << "header_size : " << header_size << std::endl;
    std::cout << "rpc_header_str : " << rpc_header_str << std::endl;
    std::cout << "service_name : " << service_name << std::endl;
    std::cout << "method_name : " << method_name << std::endl;
    std::cout << "args_str : " << args_str << std::endl;
    std::cout << "===========================================" << std::endl;


    //尋找對應的對象和方法
    auto service_it = m_serviceMap.find(service_name);
    if(service_it == m_serviceMap.end())
    {
        std::cout << "service name : " << service_name  << "is not exist"<< std::endl;
        return ;
    }

    auto method_it = service_it->second.m_methodMap.find(method_name);
    if(method_it == service_it->second.m_methodMap.end())
    {
        std::cout << "service name : " << service_name  << ":" << method_name << "is not exist"<< std::endl;
        return ;
    }

    google::protobuf::Service *service = service_it->second.m_service; //獲取service對象
    const google::protobuf::MethodDescriptor * method = method_it->second; //獲取service對象的method方法

    //生成的rpc方法調用的請求request和相應response
    google::protobuf::Message *request = service->GetRequestPrototype(method).New();
    if(!request->ParseFromString(args_str))
    {
        std::cout << "request parse error,content:" << args_str << std::endl;
    }

    google::protobuf::Message *response = service->GetResponsePrototype(method).New();
    
    //給下面的method方法的調用，綁定一個closure的回調函數
    google::protobuf::Closure *done = google::protobuf::NewCallback<RpcProvider,
                                                                    const muduo::net::TcpConnectionPtr&,
                                                                    google::protobuf::Message *>
                                                                    (this,
                                                                    &RpcProvider::SendRpcResponse,
                                                                    conn,response);


    service->CallMethod(method,nullptr,request,response,done);//用獲取到的參數去調用業務方法

}

void RpcProvider::SendRpcResponse(const muduo::net::TcpConnectionPtr& conn,google::protobuf::Message* response)
{
    std::string response_str;
    if(response->SerializeToString(&response_str))
    {
        conn->send(response_str);
    }
    else
    {
        std::cout << "response Serialize error" << std::endl;
    }
    conn->shutdown();

}