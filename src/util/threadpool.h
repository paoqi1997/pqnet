#ifndef PQNET_UTIL_THREAD_POOL_H
#define PQNET_UTIL_THREAD_POOL_H

#include <memory>
#include <queue>
#include <vector>

#include "condition.h"
#include "types.h"

namespace pqnet
{

struct Task
{
    void* run() { return func(arg); }
    pn_thread_func_t func;
    pn_thread_arg_t arg;
};

class Thread;

class ThreadPool
{
public:
    ThreadPool(std::size_t threadNumber);
    ~ThreadPool();
    void start();
    void stop();
    void addTask(Task task);
    Task take();
    bool isRunning() const { return running; }
    bool isEmpty() const { return taskqueue.empty(); }
public:
    Condition cond;
private:
    bool running;
    std::size_t tn;
    std::queue<Task> taskqueue;
    std::vector<std::unique_ptr<Thread>> pool;
};

} // namespace pqnet

#endif // PQNET_UTIL_THREAD_POOL_H
