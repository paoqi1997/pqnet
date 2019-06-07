#ifndef PQNET_CORE_TRIGGER_H
#define PQNET_CORE_TRIGGER_H

#include <cstdint>

#include <sys/epoll.h>

#include "callback.h"

namespace pqnet
{

class Trigger
{
public:
    Trigger(int _epfd, int _fd);
    int getFd() const { return fd; }
    void addToLoop() { updateEvents(EPOLL_CTL_ADD); }
    void removeFromLoop() { updateEvents(EPOLL_CTL_DEL); }
    void setRevents(std::uint32_t _revents) { revents = _revents; }
    void handleEvent();
    void likeReading() { events |= EPOLLIN; updateEvents(EPOLL_CTL_MOD); }
    void likeWriting() { events |= EPOLLOUT; updateEvents(EPOLL_CTL_MOD); }
    void omitReading() { events &= ~EPOLLIN; updateEvents(EPOLL_CTL_MOD); }
    void omitWriting() { events &= ~EPOLLOUT; updateEvents(EPOLL_CTL_MOD); }
    bool isLikeReading() { return events & EPOLLIN; }
    bool isLikeWriting() { return events & EPOLLOUT; }
    void setReadHandler(const readHandler& hd) { readhd = hd; }
    void setWriteHandler(const writeHandler& hd) { writehd = hd; }
private:
    void updateEvents(int op);
    int epfd;
    int fd;
    std::uint32_t events;
    std::uint32_t revents;
    readHandler readhd;
    writeHandler writehd;
};

} // namespace pqnet

#endif // PQNET_CORE_TRIGGER_H
