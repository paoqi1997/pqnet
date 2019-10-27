#include "threadpool.h"

ThreadPool::ThreadPool(std::size_t threadNumber) : running(false), tn(threadNumber)
{

}

ThreadPool::~ThreadPool()
{
    running = false;
    cond.notify_all();
    for (auto &t : pool) {
        if (t->joinable()) {
            t->join();
        }
    }
}

void ThreadPool::run()
{
    running = true;
    for (std::size_t i = 0; i < tn; ++i) {
        pool.emplace_back(new std::thread([this]{
            for (;;) {
                Task task;
                if (true) {
                    std::unique_lock<std::mutex> lk(mtx);
                    cond.wait(lk, [this]{ return !this->isRunning() || !this->isIdle(); });
                    if (!this->isRunning()) {
                        break;
                    }
                    task = this->take();
                }
                task.run();
            }
        }));
    }
}

void ThreadPool::addTask(Task task)
{
    if (tn == 0) {
        task.run();
    } else {
        if (true) {
            std::lock_guard<std::mutex> lk(mtx);
            taskqueue.push(task);
        }
        cond.notify_one();
    }
}

Task ThreadPool::take()
{
    Task task = taskqueue.front();
    taskqueue.pop();
    return task;
}
