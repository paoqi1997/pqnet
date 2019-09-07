#include "eventloopthread.h"

#include <cerrno>
#include <cstring>

#include "../util/logger.h"

using namespace pqnet;

EventLoopThread::EventLoopThread()
{

}

void EventLoopThread::start()
{
    if (pthread_create(&id, nullptr, routine, &m_looper) != 0) {
        ERROR(std::strerror(errno));
    }
}

void* EventLoopThread::routine(void *arg)
{
    auto looper = static_cast<EventLoop*>(arg);
    looper->loop();
    return nullptr;
}
