#ifndef PQNET_UTIL_THREAD_H
#define PQNET_UTIL_THREAD_H

#include <pthread.h>

#include "types.h"

namespace pqnet
{

class ThreadPool;

class Thread
{
public:
    Thread(ThreadPool *_poolptr);
    Thread(ThreadPool *_poolptr, pn_thread_func_t _func);
    void run();
    static void* routine(void *arg);
    pthread_t getId() const { return id; }
    ThreadPool* getPool() const { return poolptr; }
    void setFunc(pn_thread_func_t _func) { func = _func; }
private:
    pthread_t id;
    ThreadPool *poolptr;
    pn_thread_func_t func;
};

} // namespace pqnet

#endif // PQNET_UTIL_THREAD_H
