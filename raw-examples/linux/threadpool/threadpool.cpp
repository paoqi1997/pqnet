#include "threadpool.h"

void* routine(void *arg)
{
    auto pool = static_cast<ThreadPool*>(arg);
    Condition& cond = pool->Cond();
    for (;;) {
        cond.lock();
        while (pool->isRunning() && pool->isIdle()) {
            cond.wait();
        }
        if (!pool->isRunning() && pool->isIdle()) {
            cond.unlock();
            break;
        }
        Task task = pool->take();
        cond.unlock();
        task.run();
    }
    return nullptr;
}

ThreadPool::ThreadPool(std::size_t threadNumber)
    : running(false), addable(true), tn(threadNumber), pool(threadNumber)
{

}

ThreadPool::~ThreadPool()
{
    cond.notifyAll();
    for (auto id : pool) {
        if (pthread_join(id, nullptr) != 0) {
            std::cout << std::strerror(errno) << std::endl;
        }
    }
}

void ThreadPool::run()
{
    running = true;
    for (std::size_t i = 0; i < tn; ++i) {
        if (pthread_create(&pool[i], nullptr, routine, this) != 0) {
            std::cout << std::strerror(errno) << std::endl;
        }
    }
}

void ThreadPool::addTask(Task task)
{
    if (!addable)
        return;

    if (tn == 0) {
        task.run();
    } else {
        cond.lock();
        taskqueue.push(task);
        cond.unlock();
        cond.notify();
    }
}

Task ThreadPool::take()
{
    Task task = taskqueue.front();
    taskqueue.pop();
    return task;
}
