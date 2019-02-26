#include <cerrno>
#include <cstring>

#include <pthread.h>

#include "logger.h"
#include "thread.h"
#include "threadpool.h"

using namespace pqnet;

Thread::Thread(ThreadPool *_poolptr) : poolptr(_poolptr), func(nullptr)
{

}

Thread::Thread(ThreadPool *_poolptr, pn_thread_func _func) : poolptr(_poolptr), func(_func)
{

}

void Thread::run()
{
    if (func == nullptr) {
        func = routine;
    }
    if (pthread_create(&id, nullptr, func, this) != 0) {
        ERROR(std::strerror(errno));
    }
}

void* Thread::routine(void *arg)
{
    auto self = static_cast<Thread*>(arg);
    auto pool = self->getPool();
    for ( ; ; ) {
        pool->cond.lock();
        while (pool->isRunning() && pool->isIdle()) {
            pool->cond.wait();
        }
        if (!pool->isRunning()) {
            pool->cond.unlock();
            break;
        }
        auto task = pool->take();
        pool->cond.unlock();
        task.run();
    }
    return nullptr;
}
