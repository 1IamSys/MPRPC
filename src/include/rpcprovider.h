#pragma once
#include "google/protobuf/service.h"
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <google/protobuf/descriptor.h>
#include <memory>
#include <string>
#include <unordered_map>

// 要求：高并发

class RpcProvider
{
public:

    // callee 发布 RPC 方法的函数接口
    void NotifyService(google::protobuf::Service *service);

    // 启动 RPC 服务节点，开始提供 RPC 远程网络调用服务
    void Run();
private:
    // std::unique_ptr<muduo::net::TcpServer> m_tcpserverPtr;

    muduo::net::EventLoop m_eventLoop;

    //服務信息
    struct ServiceInfo
    {
        google::protobuf::Service *m_service;  //服務信息描述符
        std::unordered_map<std::string , const google::protobuf::MethodDescriptor *> m_methodMap; // 方法名字和描述符
    };

    //存儲所有服務的全部信息
    std::unordered_map<std::string,ServiceInfo> m_serviceMap;
    

    // 新的 socket 连接回调
    void OnConnection(const muduo::net::TcpConnectionPtr&);

    // 已建立连接用户的读写事件回调
    void OnMessage(const muduo::net::TcpConnectionPtr&, muduo::net::Buffer*, muduo::Timestamp);

    //closure的回調操作，用於序列化rpc相應和網絡發送
    void SendRpcResponse(const muduo::net::TcpConnectionPtr& conn,google::protobuf::Message *response);
};
