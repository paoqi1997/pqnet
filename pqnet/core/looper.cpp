#include <cerrno>
#include <cstring>

#include <sys/eventfd.h>
#include <unistd.h>

#include "../util/logger.h"
#include "looper.h"

using namespace pqnet;

Looper::Looper() : msg(0), func(nullptr)
{
    evfd = eventfd(0, EFD_NONBLOCK);
    if (evfd == -1) {
        ERROR(std::strerror(errno));
    }
    epfd = epoll_create(EPOLLSIZE);
    if (epfd == -1) {
        ERROR(std::strerror(errno));
    }
    poi.data.fd = evfd;
    poi.events = EPOLLET | EPOLLIN;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, evfd, &poi) == -1) {
        ERROR(std::strerror(errno));
    }
}

Looper::Looper(pn_thread_func_t _func) : msg(0), func(_func)
{
    evfd = eventfd(0, EFD_NONBLOCK);
    if (evfd == -1) {
        ERROR(std::strerror(errno));
    }
    epfd = epoll_create(EPOLLSIZE);
    if (epfd == -1) {
        ERROR(std::strerror(errno));
    }
    poi.data.fd = evfd;
    poi.events = EPOLLET | EPOLLIN;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, evfd, &poi) == -1) {
        ERROR(std::strerror(errno));
    }
}

// close(Looper) -> EPOLLRDHUP(Client) -> close(Client)
Looper::~Looper()
{
    if (close(epfd) == -1) {
        ERROR(std::strerror(errno));
    }
    if (close(evfd) == -1) {
        ERROR(std::strerror(errno));
    }
    for (auto conn : connpool) {
        // 关闭连接并告知客户端
        if (close(conn.first) == -1) {
            ERROR(std::strerror(errno));
        }
        //this->onCloseBySock(conn.second);
    }
}

void Looper::run()
{
    if (func == nullptr) {
        func = routine;
    }
    if (pthread_create(&id, nullptr, func, this) != 0) {
        ERROR(std::strerror(errno));
    }
}

void* Looper::routine(void *arg)
{
    auto self = static_cast<Looper*>(arg);
    for (;;) {
        int cnt = epoll_wait(self->epfd, self->evpool, EPOLLSIZE, -1);
        if (cnt == -1) {
            ERROR(std::strerror(errno));
            break;
        }
        for (int i = 0; i < cnt; ++i) {
            if (self->evpool[i].data.fd == self->evfd) {
                read(self->evfd, &self->msg, sizeof(std::uint64_t));
                if (self->msg == EV_CONN) {
                    int connfd = self->waitconns.front();
                    self->connpool[connfd] = std::make_shared<TcpConnection>(self->epfd, connfd);
                    self->connpool[connfd]->setConnectCallBack(self->conncb);
                    self->connpool[connfd]->setCloseCallBack(self->closecb);
                    self->connpool[connfd]->setMessageArrivedCallBack(self->macb);
                    self->connpool[connfd]->connectEstablished();
                }
            } else {
                auto tg = reinterpret_cast<Trigger*>(self->evpool[i].data.ptr);
                tg->setRevents(self->evpool[i].events);
                tg->handleEvent();
            }
        }
        if (self->msg == EV_EXIT) {
            INFO("Signal coming: epoll_wait exits.");
            break;
        }
    }
    return nullptr;
}