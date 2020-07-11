#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool
{
public:
    using Task = std::function<void()>;
public:
    ThreadPool(std::size_t threadNumber);
    ~ThreadPool();
    void run();
    void shutdown() { running = false; }
    template <typename F, typename... Args>
    auto addTask(F&& f, Args&&... args) -> std::future<decltype(f(args...))>
    {
        using ResType = decltype(f(args...));

        auto task = std::make_shared<std::packaged_task<ResType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<ResType> future = task->get_future();

        if (tn == 0) {
            (*task)();
        } else {
            if (true) {
                std::lock_guard<std::mutex> lk(mtx);
                taskqueue.emplace([task](){ (*task)(); });
            }
            cond.notify_one();
        }

        return future;
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
    std::mutex mtx;
    std::condition_variable cond;
    std::queue<Task> taskqueue;
    std::vector<std::unique_ptr<std::thread>> pool;
};

#endif // THREAD_POOL_H
