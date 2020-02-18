#include "eventloop.h"

#include <cerrno>
#include <cstring>

#include <sys/eventfd.h>
#include <unistd.h>

#include "../util/logger.h"
#include "../util/types.h"

using namespace pqnet;

EventLoop::EventLoop(int eventPoolSize)
    : loopFlag(false), evTrigger(new Trigger()), tmTrigger(new Trigger()),
      tmqueue(new TimerQueue()), evpool(eventPoolSize)
{
    epfd = epoll_create(eventPoolSize);
    if (epfd == -1) {
        ERROR(std::strerror(errno));
    }
    evfd = eventfd(0, EFD_NONBLOCK);
    if (evfd == -1) {
        ERROR(std::strerror(errno));
    }
    // For EvFd
    evTrigger->setEpfd(epfd);
    evTrigger->setFd(evfd);
    evTrigger->setReadHandler(std::bind(&EventLoop::handleRead, this));
    evTrigger->addToLoop();
    evTrigger->likeReading();
    // For TmFd
    tmTrigger->setEpfd(epfd);
    tmTrigger->setFd(tmqueue->getFd());
    tmTrigger->setReadHandler([this]{ tmqueue->handle(); });
    tmTrigger->addToLoop();
    tmTrigger->likeReading();
}

EventLoop::~EventLoop()
{
    if (close(epfd) == -1) {
        ERROR(std::strerror(errno));
    }
    if (close(evfd) == -1) {
        ERROR(std::strerror(errno));
    }
}

void EventLoop::loop()
{
    loopFlag = true;
    while (loopFlag) {
        int cnt = epoll_wait(epfd, this->begin(), static_cast<int>(evpool.size()), -1);
        if (cnt == -1) {
            loopFlag = false;
            if (errno == EINTR) {
                TRACE(MSG_EPOLL_WAIT_EINTR);
            } else {
                ERROR(std::strerror(errno));
            }
        } else {
            for (int i = 0; i < cnt; ++i) {
                auto tg = reinterpret_cast<Trigger*>(evpool[i].data.ptr);
                tg->setRevents(evpool[i].events);
                tg->handleEvent();
            }
        }
    }
}

TimerId EventLoop::runAt(const timerCallBack& cb, void *arg, const TimeStamp& ts)
{
    TimeStamp nowTs = now();
    if (nowTs.Int13() < ts.Int13()) {
        return tmqueue->addTimer(cb, arg, ts.Int13() - nowTs.Int13());
    } else {
        return tmqueue->addTimer(cb, arg, 0);
    }
}

TimerId EventLoop::runAfter(const timerCallBack& cb, void *arg, uint expiration)
{
    return tmqueue->addTimer(cb, arg, expiration);
}

TimerId EventLoop::runEvery(const timerCallBack& cb, void *arg, uint expiration, uint interval)
{
    return tmqueue->addTimer(cb, arg, expiration, interval);
}

void EventLoop::cancel(TimerId id)
{
    tmqueue->delTimer(id);
}

void EventLoop::handleRead()
{
    TRACE("Fd: %d, Func: EventLoop::%s", epfd, __func__);
    ssize_t n = read(evfd, &msg, sizeof(msg));
    if (n == -1) {
        ERROR(std::strerror(errno));
    } else {
        switch (msg) {
        case EV_CONN:
            this->popFn()();
            break;
        case EV_EXIT:
            loopFlag = false;
            break;
        }
    }
}
