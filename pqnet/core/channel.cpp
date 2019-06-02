#include <cerrno>
#include <cstring>

#include "../util/logger.h"
#include "channel.h"

using namespace pqnet;

Channel::Channel(int _epfd, int _fd) : epfd(_epfd), fd(_fd), events(0)
{
    events |= EPOLLET;
    events |= EPOLLRDHUP;
}

void Channel::handleEvent()
{
    if (revents & (EPOLLIN | EPOLLRDHUP)) {
        if (readhd) {
            readhd();
        }
    }
    if (revents & EPOLLOUT) {
        if (writehd) {
            writehd();
        }
    }
}

void Channel::updateEvents(int op)
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
