#ifndef PQNET_UTIL_THREAD_POOL_H
#define PQNET_UTIL_THREAD_POOL_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace pqnet
{

class ThreadPool
{
public:
    using Task = std::function<void()>;
public:
    ThreadPool(std::size_t threadNumber);
    ~ThreadPool();
    void run();
    void shutdown() { running = addable = false; }
    template <typename F, typename... Args>
    void addTask(F&& f, Args&&... args)
    {
        if (!addable) {
            return;
        }

        auto task = std::make_shared<Task>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        if (tn == 0) {
            (*task)();
        } else {
            if (true) {
                std::lock_guard<std::mutex> lk(mtx);
                taskqueue.emplace([task](){ (*task)(); });
            }
            cond.notify_one();
        }
    }
    bool isRunning() const { return running; }
    bool isIdle() const { return taskqueue.empty(); }
private:
    Task take() {
        auto task = taskqueue.front();
        taskqueue.pop();
        return task;
    }
private:
    std::size_t tn;
    std::atomic<bool> running;
    std::atomic<bool> addable;
    std::mutex mtx;
    std::condition_variable cond;
    std::queue<Task> taskqueue;
    std::vector<std::unique_ptr<std::thread>> pool;
};

} // namespace pqnet

#endif // PQNET_UTIL_THREAD_POOL_H
