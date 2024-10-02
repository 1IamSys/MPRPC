#include "mprpcapplication.h"
#include <iostream>
#include <unistd.h>
#include <string>

MprpcConfig MprpcApplication::my_config;

void ShowArgsHelp()
{
    std::cout << "format: command -i <configfile>" << std::endl;
}


void MprpcApplication::init(int argc,char **argv) //框架的初始化工作(加載配置文件)
{
    if(argc <2)
    {
        ShowArgsHelp();
        exit(EXIT_FAILURE);
    }

    int c = 0;
    std::string config_file;
    while((c = getopt(argc,argv,"i:")) != -1)
    {
        switch (c)
        {
        case 'i'://遇到-i則將後面的字符串（配置文件路徑）傳給config_file
            config_file = optarg;
            break;
        case '?': //遇到無效字符
            ShowArgsHelp();
            exit(EXIT_FAILURE);
        case ':': //有-i但是後面沒有提供參數
            ShowArgsHelp();
            exit(EXIT_FAILURE);
        default:
            break;
        }
    }

    my_config.loadConfigFile(config_file.c_str());
    //加載配置文件

}


MprpcApplication & MprpcApplication::GetInstance()//獲取框架的唯一單例
{
    static MprpcApplication app;
    return app;
}

MprpcConfig& MprpcApplication::GetMyConfig()
{
    return my_config;
}