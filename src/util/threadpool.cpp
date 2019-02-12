#include <cerrno>
#include <cstring>

#include <pthread.h>

#include "logger.h"
#include "threadpool.h"

using namespace pqnet;

ThreadPool::ThreadPool(std::size_t threadNumber) : al(this), running(false), tn(threadNumber)
{
    for (std::size_t i = 0; i < tn; ++i) {
        pool.emplace_back(new Thread(this));
    }
}

ThreadPool::ThreadPool(std::size_t threadNumber, pn_thread_func func)
    : al(this), running(false), tn(threadNumber)
{
    for (std::size_t i = 0; i < tn; ++i) {
        pool.emplace_back(new Thread(this, func));
    }
}

ThreadPool::~ThreadPool()
{

}

void ThreadPool::start()
{
    al.run();
    for (auto &t : pool) {
        t->run();
    }
    running = true;
}

void ThreadPool::stop()
{
    running = false;
    al.cond.notify();
    if (pthread_join(al.getId(), nullptr) != 0) {
        ERROR(std::strerror(errno));
    }
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
