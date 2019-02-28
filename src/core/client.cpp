#include <cerrno>
#include <cstdio>
#include <cstring>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../util/logger.h"
#include "client.h"
#include "socket.h"

using namespace pqnet;

TcpClient::TcpClient(const char *servname, std::uint16_t port)
    : addr(servname, port), running(false)
{
    // socket
    sockfd = new_socket();
    if (sockfd == -1) {
        ERROR(std::strerror(errno));
    }
    // connect
    auto addrptr = reinterpret_cast<struct sockaddr*>(addr.getAddr());
    if (connect(sockfd, addrptr, sizeof(struct sockaddr)) == -1) {
        ERROR(std::strerror(errno));
    }
    setNonBlock(sockfd, true);
    setReuseAddr(sockfd, true);
    setReusePort(sockfd, true);
    connptr = std::make_shared<TcpConnection>(sockfd);
    epfd = epoll_create(CLI_EVS);
    if (epfd == -1) {
        ERROR(std::strerror(errno));
    }
    poi.data.fd = sockfd;
    poi.events = EPOLLET | EPOLLRDHUP | EPOLLIN;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &poi) == -1) {
        ERROR(std::strerror(errno));
    }
    poi.data.fd = fileno(stdin);
    poi.events = EPOLLET | EPOLLIN;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fileno(stdin), &poi) == -1) {
        ERROR(std::strerror(errno));
    }
}

// close(Client) -> EPOLLRDHUP(Looper)
TcpClient::~TcpClient()
{
    if (close(epfd) == -1) {
        ERROR(std::strerror(errno));
    }
    // 关闭连接并告知服务端
    if (close(sockfd) == -1) {
        ERROR(std::strerror(errno));
    }
}

void TcpClient::run()
{
    this->onConnect(connptr);
    running = true;
    while (running) {
        int cnt = epoll_wait(epfd, evpool, CLI_EVS, -1);
        if (cnt == -1) {
            if (errno == EINTR) {
                // Has been in shutdown state
                this->onCloseBySock(connptr);
                INFO("Signal coming: epoll_wait exits.");
            } else {
                ERROR(std::strerror(errno));
                break;
            }
        }
        for (int i = 0; i < cnt; ++i) {
            // 服务端关闭连接
            if (evpool[i].events & EPOLLRDHUP) {
                this->shutdown();
                this->onCloseByPeer(connptr);
            }
            else if (evpool[i].data.fd == fileno(stdin)) {
                this->handleIn(connptr);
            }
            else {
                if (evpool[i].events & EPOLLIN) {
                    this->onRead(connptr);
                    poi.data.fd = sockfd;
                    poi.events = EPOLLET | EPOLLRDHUP | EPOLLOUT;
                    if (epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &poi) == -1) {
                        ERROR(std::strerror(errno));
                    }
                }
                if (evpool[i].events & EPOLLOUT) {
                    this->onMessage(connptr);
                    poi.data.fd = sockfd;
                    poi.events = EPOLLET | EPOLLRDHUP | EPOLLIN;
                    if (epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &poi) == -1) {
                        ERROR(std::strerror(errno));
                    }
                }
            }
        }
    }
}
