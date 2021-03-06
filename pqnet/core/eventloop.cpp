#include <cerrno>
#include <cstdint>
#include <cstring>

#include <sys/eventfd.h>
#include <unistd.h>

#include "../util/defines.h"
#include "../util/logger.h"
#include "eventloop.h"

using namespace pqnet;

EventLoop::EventLoop(int eventPoolSize)
    : loopFlag(false), evTrigger(new Trigger()),
      tmqueue(new TimerQueue()), evpool(eventPoolSize)
{
    epfd = epoll_create1(EPOLL_CLOEXEC);
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
        int cnt = epoll_wait(
            epfd, this->begin(), static_cast<int>(evpool.size()), tmqueue->getExpire()
        );
        tmqueue->handle();
        if (cnt == -1) {
            loopFlag = false;
            if (errno == EINTR) {
                DEBUG(MSG_EPOLL_WAIT_EINTR);
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
        runFunctors();
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

void EventLoop::runFunctors()
{
    std::vector<Functor> funcs;

    if (true) {
        std::lock_guard<std::mutex> lk(mtx);
        funcs.swap(functors);
    }

    for (auto& func : funcs) {
        func();
    }
}

void EventLoop::pushFunctor(const Functor& fn)
{
    if (true) {
        std::lock_guard<std::mutex> lk(mtx);
        functors.push_back(fn);
    }
    this->wake();
}

void EventLoop::wake()
{
    DEBUG("LooperFd: %d, Func: EventLoop::%s", epfd, __func__);
    std::uint64_t msg = 1;
    ssize_t n = write(evfd, &msg, sizeof(msg));
    if (n == -1) {
        ERROR(std::strerror(errno));
    }
}

void EventLoop::handleRead()
{
    DEBUG("LooperFd: %d, Func: EventLoop::%s", epfd, __func__);
    std::uint64_t msg = 1;
    ssize_t n = read(evfd, &msg, sizeof(msg));
    if (n == -1) {
        ERROR(std::strerror(errno));
    }
}
