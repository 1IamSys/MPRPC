#pragma once
#include<unordered_map>
#include <string>

//框架讀取配置文件類

class MprpcConfig
{
public:
    void loadConfigFile(const char * config_file);
    std::string Load(const std::string &key);
private:
    void trim(std::string &str);
    std::unordered_map<std::string,std::string> m_configmap;
};
