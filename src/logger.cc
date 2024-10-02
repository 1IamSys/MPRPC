#include "logger.h"
#include <time.h>
#include <iostream>

// 設置日誌級別
void Logger::SetLogLevel(LogLevel level)
{
    m_loglevel = level;
}


Logger::Logger()
{
    //啓動專門的寫日誌線程
    std::thread writeLogTask([&]()
    {
        for(;;)
        {
            time_t now = time(nullptr);
            tm *nowtm = localtime(&now);

            char filename[128];
            sprintf(filename,"%d-%d-%d-log.txt",nowtm->tm_year+1900,nowtm->tm_mon+1,nowtm->tm_mday);

            FILE *pf = fopen(filename,"a+");
            if(pf == nullptr)
            {
                std::cout << "filename:" << filename << "open error" <<std::endl;
            }
            std::string msg = m_lckQue.Pop(); //取出日誌

            char time_buf[64];
            sprintf(time_buf,"%d:%d:%d => [%s]",
                                                nowtm->tm_hour,
                                                nowtm->tm_min,
                                                nowtm->tm_sec,
                                                (m_loglevel == INFO ? "info" : "error"));//每行都插入飾分秒信息
            msg.insert(0,time_buf);
            msg.append("\n");

            fputs(msg.c_str(),pf);
            fclose(pf);
        }
    }
    );
    writeLogTask.detach();
}

// 寫日誌
void Logger::Log(std::string msg)
{
    
    m_lckQue.Push(msg);
}

Logger & Logger::GetInstance()
{
    static Logger instance;
    return instance;
}