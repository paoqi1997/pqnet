#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

struct Task
{
    void run() { func(arg); }
    std::function<void(void*)> func;
    void *arg;
};

class ThreadPool
{
public:
    ThreadPool(std::size_t threadNumber);
    ~ThreadPool();
    void run();
    void shutdown() { running = false; }
    void addTask(Task task);
    Task take();
    bool isRunning() const { return running; }
    bool isIdle() const { return taskqueue.empty(); }
private:
    std::atomic<bool> running;
    std::size_t tn;
    std::queue<Task> taskqueue;
    std::mutex mtx;
    std::condition_variable cond;
    std::vector<std::unique_ptr<std::thread>> pool;
};

#endif // THREAD_POOL_H
