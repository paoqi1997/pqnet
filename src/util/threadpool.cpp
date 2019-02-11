#include <cerrno>
#include <cstring>

#include <pthread.h>

#include "logger.h"
#include "thread.h"
#include "threadpool.h"

namespace pqnet
{

ThreadPool::ThreadPool(std::size_t threadNumber) : tn(threadNumber), running(false)
{
    for (std::size_t i = 0; i < tn; ++i) {
        pool.emplace_back(new Thread(this));
    }
}

ThreadPool::~ThreadPool()
{

}

void ThreadPool::start()
{
    for (auto &t : pool) {
        t->run();
    }
    running = true;
}

void ThreadPool::stop()
{
    running = false;
    cond.notifyAll();
    for (auto &t : pool) {
        if (pthread_join(t->getId(), nullptr) != 0) {
            ERROR(std::strerror(errno));
        }
    }
}

void ThreadPool::addTask(Task task)
{
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

} // namespace pqnet
