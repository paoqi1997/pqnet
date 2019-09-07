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
    EventLoopThreadPool(std::size_t evThreadNumber);
    ~EventLoopThreadPool();
    void start();
private:
    std::size_t m_evThreadNumber;
    std::vector<std::unique_ptr<EventLoopThread>> evThreadPool;
};

} // namespace pqnet

#endif // PQNET_CORE_EVENTLOOP_THREAD_POOL_H
