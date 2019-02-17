#include <cerrno>
#include <cstring>

#include <sys/eventfd.h>
#include <unistd.h>

#include "../util/logger.h"
#include "looper.h"

using namespace pqnet;

Looper::Looper(LooperPool *_poolptr) : poolptr(_poolptr), func(nullptr)
{
    evfd = eventfd(0, EFD_NONBLOCK);
    if (evfd == -1) {
        ERROR(std::strerror(errno));
    }
    epfd = epoll_create(MAXEVENTS);
    if (epfd == -1) {
        ERROR(std::strerror(errno));
    }
    poi.data.fd = evfd;
    poi.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, evfd, &poi) == -1) {
        ERROR(std::strerror(errno));
    }
}

Looper::Looper(LooperPool *_poolptr, pn_thread_func _func) : poolptr(_poolptr), func(_func)
{
    evfd = eventfd(0, EFD_NONBLOCK);
    if (evfd == -1) {
        ERROR(std::strerror(errno));
    }
    epfd = epoll_create(MAXEVENTS);
    if (epfd == -1) {
        ERROR(std::strerror(errno));
    }
    poi.data.fd = evfd;
    poi.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, evfd, &poi) == -1) {
        ERROR(std::strerror(errno));
    }
}

Looper::~Looper()
{
    
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
    auto pool = self->getPool();
    std::printf("RUN!\n");
    std::printf("%d\n", self->evfd);
    if (self->conncb == nullptr) {
        std::printf("ThreadNoConn!\n");
    }
    if (self->closecb == nullptr) {
        std::printf("ThreadLoopNoClose!\n");
    }
    if (self->readcb == nullptr) {
        std::printf("ThreadLoopNoRead!\n");
    }
    if (self->msgcb == nullptr) {
        std::printf("ThreadLoopNoMsg!\n");
    }
    for ( ; ; ) {
        int cnt = epoll_wait(self->epfd, self->evpool, MAXEVENTS, -1);
        if (cnt == -1) {
            ERROR(std::strerror(errno));
            break;
        }
        for (int i = 0; i < cnt; ++i) {
            if (self->evpool[i].data.fd == self->evfd) {
                std::uint64_t msg;
                read(self->evfd, &msg, sizeof(std::uint64_t));
                int connfd = self->waitconns.front();
                self->waitconns.pop();
                self->poi.data.fd = connfd;
                self->poi.events = EPOLLET | EPOLLRDHUP | EPOLLIN;
                if (epoll_ctl(self->epfd, EPOLL_CTL_ADD, connfd, &self->poi) == -1) {
                    ERROR(std::strerror(errno));
                }
                self->connpool.insert(
                    std::pair<int, TcpConnPtr>(connfd, std::make_shared<TcpConnection>(connfd))
                );
                self->onConnect(self->connpool[connfd]);
                std::printf("%ld %d Connect!\n", connfd, pthread_self());
            }
            else if (self->evpool[i].events & EPOLLRDHUP) {
                int connfd = self->evpool[i].data.fd;
                if (epoll_ctl(self->epfd, EPOLL_CTL_DEL, connfd, nullptr) == -1) {
                    ERROR(std::strerror(errno));
                }
                self->onClose(self->connpool[connfd]);
                self->connpool.erase(connfd);
                std::printf("%ld %d Bye!\n", connfd, pthread_self());
            }
            else if (self->evpool[i].events & EPOLLIN) {
                int connfd = self->evpool[i].data.fd;
                self->onRead(self->connpool[connfd]);
                self->poi.data.fd = connfd;
                self->poi.events = EPOLLET | EPOLLRDHUP | EPOLLOUT;
                if (epoll_ctl(self->epfd, EPOLL_CTL_MOD, connfd, &self->poi) == -1) {
                    ERROR(std::strerror(errno));
                }
                std::printf("%d Read!\n", connfd);
            }
            else if (self->evpool[i].events & EPOLLOUT) {
                int connfd = self->evpool[i].data.fd;
                self->onMessage(self->connpool[connfd]);
                self->poi.data.fd = connfd;
                self->poi.events = EPOLLET | EPOLLRDHUP | EPOLLIN;
                if (epoll_ctl(self->epfd, EPOLL_CTL_MOD, connfd, &self->poi) == -1) {
                    ERROR(std::strerror(errno));
                }
                std::printf("%d Message!\n", connfd);
            }
        } 
    }
    self->shutdown();
}

void Looper::shutdown()
{
    for (auto conn : connpool) {
        this->onClose(conn.second);
    }
}
