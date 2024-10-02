#pragma once
#include <google/protobuf/service.h>
#include <string>

class MprpcController:public google::protobuf::RpcController
{
public:
    MprpcController();
    void Reset() ;

    bool Failed() const ;
    void SetFailed(const std::string& reason) ;
    std::string ErrorText() const ;

    //還未實現功能
    void StartCancel() ;
    bool IsCanceled() const ;
    void NotifyOnCancel(google::protobuf::Closure* callback) ;

private:
    bool m_failed;
    std::string m_errText;
};