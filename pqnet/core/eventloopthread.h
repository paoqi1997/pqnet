#ifndef PQNET_CORE_EVENTLOOP_THREAD_H
#define PQNET_CORE_EVENTLOOP_THREAD_H

#include <memory>
#include <thread>

#include "eventloop.h"

namespace pqnet
{

class EventLoopThread
{
public:
    EventLoopThread();
    void start();
    void join();
    std::size_t getId() const;
    EventLoop* getEventLoop() { return &looper; }
private:
    EventLoop looper;
    std::unique_ptr<std::thread> thd;
};

} // namespace pqnet

#endif // PQNET_CORE_EVENTLOOP_THREAD_H
