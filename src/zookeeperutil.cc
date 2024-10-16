#include "zookeeperutil.h"
#include "mprpcapplication.h"

#include <semaphore.h>
#include <iostream>

void golbal_watcher(zhandle_t *zh,int type,int state,const char *path,void *watcherCtx)
{
    if(type == ZOO_SESSION_EVENT)
    {
        if(state == ZOO_CONNECTED_STATE)
        {
            sem_t *sem = (sem_t *)zoo_get_context(zh);
            sem_post(sem);
        }
    }
}


ZKclient::ZKclient():m_zhandle(nullptr)
{
}

ZKclient::~ZKclient()
{
    if(m_zhandle != nullptr)
    {
        zookeeper_close(m_zhandle);
    }
}

void ZKclient::start()
{
    std::string ip = MprpcApplication::GetInstance().GetMyConfig().Load("zookeeperip");
    std::string port = MprpcApplication::GetInstance().GetMyConfig().Load("zookeeperport");

    std::string host = ip + ":" + port;

    m_zhandle = zookeeper_init(host.c_str(),golbal_watcher,3000,nullptr,nullptr,0);

    if(m_zhandle == nullptr)
    {
        std::cout << "zookeeper_init error" << std::endl;
        exit(EXIT_FAILURE);
    }

    sem_t sem;
    sem_init(&sem,0,0);
    zoo_set_context(m_zhandle,&sem);
    
    sem_wait(&sem);
    std::cout << "zookeeper_init success" << std::endl;
}

//
void ZKclient::creat(const char *path,const char *data,int datalen,int state)
{
    char path_buff[128];
    int bufflen = sizeof(path_buff);
    int flag;

    flag = zoo_exists(m_zhandle, path, 0, nullptr); // 查看节点是否存在
    if (flag == ZNONODE)                            // 节点不存在
    {
        // 根据指定的参数创建znode节点
        flag = zoo_create(m_zhandle, path, data, datalen, &ZOO_OPEN_ACL_UNSAFE, state, path_buff, bufflen);
        if (flag == ZOK)
        {
            std::cout << "znode create success ... path:" << path << std::endl;
        }
        else
        {
            std::cout << "flag:" << flag << std::endl;
            std::cout << "znode create error ... path:" << path << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

std::string GetData(const char *path)
{
    char buffer[64] = {0};
    int bufferlen = sizeof(buffer);
    int flag = zoo_get(m_zhandle, path, 0, buffer, &bufferlen, nullptr);
    if (flag != ZOK)
    {
        std::cout << "get znode error... path:" << path << std::endl;
        return "";
    }
    else
    {
        return buffer;
    }
}