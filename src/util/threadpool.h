#ifndef PQNET_UTIL_THREAD_POOL_H
#define PQNET_UTIL_THREAD_POOL_H

#include <memory>
#include <queue>
#include <vector>

#include "condition.h"
#include "thread.h"
#include "types.h"

namespace pqnet
{

struct Task
{
    void run() { func(arg); }
    pn_task_func func;
    pn_task_arg arg;
};

class ThreadPool
{
public:
    ThreadPool(std::size_t threadNumber);
    ThreadPool(std::size_t threadNumber, pn_thread_func func);
    ~ThreadPool();
    void run();
    void shutdown() { running = false; }
    void addTask(Task task);
    Task take();
    bool isRunning() const { return running; }
    bool isIdle() const { return taskqueue.empty(); }
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
