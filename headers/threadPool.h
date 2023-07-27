#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <functional>

using namespace std;

class ThreadPool {
public:
    ThreadPool(size_t numThreads);

    template <class F, class... Args>
    void enqueue(F&& f, Args&&... args);

    ~ThreadPool();

private:
    vector<std::thread> workers;
    queue<std::function<void()>> tasks;
    mutex queueMutex;
    condition_variable condition;
    bool stop;
};

template <class F, class... Args>
void ThreadPool::enqueue(F &&f, Args &&...args)
{
    {
        unique_lock<std::mutex> lock(queueMutex);
        tasks.emplace([f, args...]
                      { f(args...); });
    }
    condition.notify_one();
}

#endif