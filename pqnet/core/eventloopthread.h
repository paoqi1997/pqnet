#ifndef PQNET_CORE_EVENTLOOP_THREAD_H
#define PQNET_CORE_EVENTLOOP_THREAD_H

#include <pthread.h>

#include "eventloop.h"

namespace pqnet
{

class EventLoopThread
{
public:
    EventLoopThread();
    pthread_t getId() const { return id; }
    EventLoop* getEventLoop() { return &looper; }
    void start();
    static void* routine(void *arg);
private:
    pthread_t id;
    EventLoop looper;
};

} // namespace pqnet

#endif // PQNET_CORE_EVENTLOOP_THREAD_H
