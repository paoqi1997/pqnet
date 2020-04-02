#include <cerrno>
#include <cstring>

#include "../util/logger.h"
#include "eventloopthread.h"

using namespace pqnet;

EventLoopThread::EventLoopThread()
{

}

void EventLoopThread::start()
{
    if (pthread_create(&id, nullptr, routine, &looper) != 0) {
        ERROR(std::strerror(errno));
    }
}

void* EventLoopThread::routine(void *arg)
{
    auto lp = static_cast<EventLoop*>(arg);
    lp->loop();
    return nullptr;
}
