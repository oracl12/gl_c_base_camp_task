#include <vector>
#include <queue>
#include <mutex>
#include <iostream>
#include <thread>
#include <condition_variable>
#include <functional>
#include "headers/threadPool.h"

using namespace std;

ThreadPool::ThreadPool(size_t numThreads) : stop(false)
{
    for (size_t i = 0; i < numThreads; ++i)
    {
        workers.emplace_back([this]
                             {
                while (true) {
                    function<void()> task;
                    {
                        unique_lock<mutex> lock(queueMutex);
                        condition.wait(lock, [this] { return stop || !tasks.empty(); });

                        if (stop && tasks.empty()) {
                            return;
                        }

                        task = move(tasks.front());
                        tasks.pop();
                    }

                    task();
                } });
    }
}

ThreadPool::~ThreadPool()
{
    {
        unique_lock<mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();

    for (thread &worker : workers)
    {
        worker.join();
    }
}
