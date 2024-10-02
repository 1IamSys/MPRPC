#pragma once
#include "mprpcconfig.h"
#include "mprpcchannel.h"
#include "mprpccontroller.h"

class MprpcApplication
{
public:
    
    static void init(int argc,char **argv); //框架的初始化工作

    static MprpcApplication & GetInstance();//獲取框架的唯一單例

    static MprpcConfig& GetMyConfig();

private:
    static MprpcConfig my_config;
    MprpcApplication() {};
    
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication(MprpcApplication&&) = delete ;     //保證單例模式

};