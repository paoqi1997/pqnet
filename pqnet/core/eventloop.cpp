#include "eventloop.h"

#include <cerrno>
#include <cstring>

#include <sys/eventfd.h>
#include <unistd.h>

#include "../util/logger.h"
#include "../util/types.h"

using namespace pqnet;

EventLoop::EventLoop(int eventPoolSize)
    : loopFlag(false), evTrigger(new Trigger()), evpool(eventPoolSize)
{
    epfd = epoll_create(eventPoolSize);
    if (epfd == -1) {
        ERROR(std::strerror(errno));
    }
    evfd = eventfd(0, EFD_NONBLOCK);
    if (evfd == -1) {
        ERROR(std::strerror(errno));
    }
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
