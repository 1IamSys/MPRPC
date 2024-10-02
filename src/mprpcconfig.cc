#include "mprpcconfig.h"
#include <iostream>


//去掉字符串前後空格
void MprpcConfig::trim(std::string &str)
{
    int index = str.find_first_not_of(" ");
    if(index != -1)
    {
        str = str.substr(index,str.size()-index);
    }


    index = str.find_last_not_of(" ");
    if(index != -1)
    {
        str = str.substr(0,index+1);
    }

}

void MprpcConfig::loadConfigFile(const char * config_file)
{
    FILE *pf = fopen(config_file,"r");

    if(pf == nullptr)
    {
        std::cout << "configfile is not exit"<< std::endl;
        exit(EXIT_FAILURE);
    }


    while(!feof(pf))
    {
        char buff[512];
        fgets(buff,512,pf);

        std::string s_buff(buff);
        //去掉兩邊空格
        trim(s_buff);
        if(s_buff[0] == '#' || s_buff.empty())
            continue;

        int index = s_buff.find('=');
        if(index == -1)
            continue;
        std::string key = s_buff.substr(0,index);
        trim(key);

        int endindex = s_buff.find('\n',index);
        std::string value = s_buff.substr(index+1,endindex-index-1);
        trim(value);
        m_configmap.insert({key,value});
    }

    // std::cout << "rpcserverip: " << Load("rpcserverip") <<std::endl;
    // std::cout << "rpcserverport: " << Load("rpcserverport") <<std::endl;
    // std::cout << "zookeeperip: " << Load("zookeeperip") <<std::endl;
    // std::cout << "zookeeperport: " << Load("zookeeperport") <<std::endl;
}


std::string MprpcConfig::Load(const std::string &key)
{
    auto it = m_configmap.find(key);
    if(it == m_configmap.end())
        return "";
    return it->second;

}