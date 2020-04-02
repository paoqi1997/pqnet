#include <cerrno>
#include <cstring>

#include <pthread.h>

#include "../util/logger.h"
#include "eventloopthreadpool.h"

using namespace pqnet;

EventLoopThreadPool::EventLoopThreadPool(std::size_t _evThreadCount)
    : currIndex(0), evThreadCount(_evThreadCount)
{
    for (std::size_t i = 0; i < evThreadCount; ++i) {
        evThreadPool.emplace_back(new EventLoopThread());
    }
}

EventLoopThreadPool::~EventLoopThreadPool()
{
    for (auto& evThread : evThreadPool) {
        if (pthread_join(evThread->getId(), nullptr) != 0) {
            ERROR(std::strerror(errno));
        }
    }
}

void EventLoopThreadPool::start()
{
    for (auto& evThread : evThreadPool) {
        evThread->start();
    }
}

EventLoop* EventLoopThreadPool::getEventLoopByIndex(std::size_t index)
{
    return evThreadPool[index]->getEventLoop();
}

EventLoop* EventLoopThreadPool::getNextEventLoop()
{
    auto looper = evThreadPool[currIndex++]->getEventLoop();
    if (currIndex >= evThreadCount) {
        currIndex = 0;
    }
    return looper;
}
