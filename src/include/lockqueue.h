#pragma once
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>


//異步寫日誌的日誌隊列
template <typename T>
class LockQueue
{
public:
    //多個worker線程都會寫日誌queue
    void Push(const T &data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(data);
        m_condvariable.notify_one();
    }

    //一個日誌線程讀queue，並且寫入日誌文件
    T Pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while(m_queue.empty())
        {
            m_condvariable.wait(lock);
        }
        T data = m_queue.front();
        m_queue.pop();
        return data;
    }

private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_condvariable;
};