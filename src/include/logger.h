#pragma once
#include "lockqueue.h"
#include <string>

enum LogLevel
{
    INFO, //普通信息
    ERROR, //錯誤信息
};

//Mprpc框架提供的日誌系統
class Logger
{
public:
    // 設置日誌級別
    void SetLogLevel(LogLevel level);

    // 寫日誌
    void Log(std::string msg);

    static Logger& GetInstance();
private:
    int m_loglevel; //記錄日誌級別
    LockQueue<std::string> m_lckQue; //日誌緩衝隊列

    Logger();
    Logger(const Logger&) = delete;
    Logger(Logger &&) = delete;
};



#define LOG_INFO(logmsgformat, ...)\
    do \
    {  \
        Logger &logger = Logger::GetInstance();\
        logger.SetLogLevel(INFO);\
        char c[1024] = {0};\
        snprintf(c,1024,logmsgformat,##__VA_ARGS__);\
        logger.Log(c);\
    } while(0);

#define LOG_ERR(logmsgformat, ...)\
    do \
    {  \
        Logger &logger = Logger::GetInstance();\
        logger.SetLogLevel(ERROR);\
        char c[1024] = {0};\
        snprintf(c,1024,logmsgformat,##__VA_ARGS__);\
        logger.Log(c);\
    } while(0);



