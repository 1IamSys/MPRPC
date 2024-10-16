#pragma once

#include <string>
#include <zookeeper/zookeeper.h>

class ZKclient
{
public:
    ZKclient();
    ~ZKclient();

    void start();

    //
    void creat(const char *path,
               const char *data,
               int datalen,
               int state = 0 
                );
    std::string GetData(const char *path);

private:

    zhandle_t *m_zhandle;
};