#include <cerrno>
#include <cstring>

#include "../util/logger.h"
#include "trigger.h"

using namespace pqnet;

Trigger::Trigger()
    : epfd(-1), fd(-1), handling(false), events(EPOLLET)
{

}

Trigger::Trigger(int _epfd, int _fd)
    : epfd(_epfd), fd(_fd), handling(false), events(EPOLLET)
{

}

void Trigger::handleEvent()
{
    handling = true;
    if (revents & EPOLLIN) {
        if (readhd) {
            readhd();
        }
    }
    if (revents & EPOLLOUT) {
        if (writehd) {
            writehd();
        }
    }
    handling = false;
}

void Trigger::updateEvents(int op)
{
    if (op == EPOLL_CTL_DEL) {
        if (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, nullptr) != 0) {
            ERROR(std::strerror(errno));
        }
    } else {
        struct epoll_event ev;
        ev.data.ptr = this;
        ev.events = events;
        switch (op) {
        case EPOLL_CTL_ADD:
            if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) != 0) {
                ERROR(std::strerror(errno));
            }
            break;
        case EPOLL_CTL_MOD:
            if (epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) != 0) {
                ERROR(std::strerror(errno));
            }
            break;
        };
    }
}
