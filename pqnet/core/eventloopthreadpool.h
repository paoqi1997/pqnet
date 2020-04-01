#ifndef PQNET_CORE_EVENTLOOP_THREAD_POOL_H
#define PQNET_CORE_EVENTLOOP_THREAD_POOL_H

#include <memory>
#include <vector>

#include "eventloopthread.h"

namespace pqnet
{

class EventLoopThreadPool
{
public:
    EventLoopThreadPool(std::size_t _evThreadCount);
    ~EventLoopThreadPool();
    void start();
    std::size_t size() const { return evThreadPool.size(); }
    EventLoop* getEventLoopByIndex(std::size_t index);
    EventLoop* getNextEventLoop();
private:
    std::size_t currIndex;
    std::size_t evThreadCount;
    std::vector<std::unique_ptr<EventLoopThread>> evThreadPool;
};

} // namespace pqnet

#endif // PQNET_CORE_EVENTLOOP_THREAD_POOL_H
