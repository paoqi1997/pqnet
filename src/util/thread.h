#ifndef PQNET_UTIL_THREAD_H
#define PQNET_UTIL_THREAD_H

#include "threadpool.h"
#include "types.h"

namespace pqnet
{

class Thread
{
public:
    Thread(ThreadPool *_poolptr);
    Thread(ThreadPool *_poolptr, pn_thread_func _func);
    ~Thread();
    void run();
    static void *routine(void *arg);
    void setFunc(pn_thread_func _func);
    pthread_t getId() const { return id; }
    ThreadPool* getPool() const { return poolptr; }
private:
    pthread_t id;
    ThreadPool *poolptr;
    pn_thread_func func;
};

} // namespace pqnet

#endif // PQNET_UTIL_THREAD_H
