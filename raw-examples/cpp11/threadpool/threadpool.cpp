#include "threadpool.h"

ThreadPool::ThreadPool(std::size_t threadNumber)
    : tn(threadNumber), running(false), addable(true)
{

}

ThreadPool::~ThreadPool()
{
    cond.notify_all();
    for (auto& t : pool) {
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
                    if (!this->isRunning() && this->isIdle()) {
                        break;
                    }
                    task = this->take();
                }
                task();
            }
        }));
    }
}
