#include "eventloop.h"

#include <cerrno>
#include <cstring>

#include <sys/eventfd.h>
#include <unistd.h>

#include "../util/logger.h"
#include "../util/types.h"

using namespace pqnet;

EventLoop::EventLoop(int eventPoolSize) : loopFlag(false), evTrigger(new Trigger()), evpool(eventPoolSize)
{
    m_epfd = epoll_create(eventPoolSize);
    if (m_epfd == -1) {
        ERROR(std::strerror(errno));
    }
    m_evfd = eventfd(0, EFD_NONBLOCK);
    if (m_evfd == -1) {
        ERROR(std::strerror(errno));
    }
    evTrigger->setFds(m_epfd, m_evfd);
    evTrigger->setReadHandler(std::bind(&EventLoop::handleRead, this));
    evTrigger->addToLoop();
    evTrigger->likeReading();
}

EventLoop::~EventLoop()
{
    if (close(m_epfd) == -1) {
        ERROR(std::strerror(errno));
    }
    if (close(m_evfd) == -1) {
        ERROR(std::strerror(errno));
    }
}

void EventLoop::loop()
{
    loopFlag = true;
    while (loopFlag) {
        int cnt = epoll_wait(m_epfd, this->begin(), static_cast<int>(evpool.size()), -1);
        if (cnt == -1) {
            loopFlag = false;
            if (errno == EINTR) {
                TRACE("epoll_wait is interrupted by a signal handler.");
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
    TRACE("Func: EventLoop::%s", __func__);
    ssize_t n = read(m_evfd, &msg, sizeof(msg));
    if (n == -1) {
        ERROR(std::strerror(errno));
    } else {
        if (msg == EV_EXIT) {
            loopFlag = false;
        } else if (msg == EV_CONN) {
            auto fn = this->popFn();
            fn();
        }
    }
}
