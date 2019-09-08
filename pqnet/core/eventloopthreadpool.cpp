#include "eventloopthreadpool.h"

#include <cerrno>
#include <cstring>

#include <pthread.h>

#include "../util/logger.h"

using namespace pqnet;

EventLoopThreadPool::EventLoopThreadPool(std::size_t evThreadNumber)
    : m_index(0), m_evThreadNumber(evThreadNumber)
{
    for (std::size_t i = 0; i < m_evThreadNumber; ++i) {
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
    TRACE("SIZE: %d", evThreadPool.size());
    for (auto& evThread : evThreadPool) {
        evThread->start();
        TRACE("evThread: %d", evThread->getEventLoop()->getFd());
    }
}

int EventLoopThreadPool::getEvfdByIndex(std::size_t index)
{
    return evThreadPool[index]->getEventLoop()->getEvfd();
}

EventLoop* EventLoopThreadPool::getNextLoop()
{
    m_index = m_index < m_evThreadNumber ? m_index + 1 : 0;
    return evThreadPool[m_index]->getEventLoop();
}
