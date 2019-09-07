#include "eventloopthreadpool.h"

#include <cerrno>
#include <cstring>

#include <pthread.h>

#include "../util/logger.h"

using namespace pqnet;

EventLoopThreadPool::EventLoopThreadPool(std::size_t evThreadNumber)
    : m_evThreadNumber(evThreadNumber)
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
    for (auto& evThread : evThreadPool) {
        evThread->start();
    }
}
