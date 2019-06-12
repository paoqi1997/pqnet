#include <cassert>
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
    auto addrptr = reinterpret_cast<struct sockaddr*>(addr.getPtr());
    if (connect(sockfd, addrptr, sizeof(struct sockaddr)) == -1) {
        ERROR(std::strerror(errno));
    }
    setNonBlock(sockfd, true);
    setReuseAddr(sockfd, true);
    setReusePort(sockfd, true);
    epfd = epoll_create(CLI_EVS);
    if (epfd == -1) {
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
    connptr = std::make_shared<TcpConnection>(epfd, sockfd);
    connptr->setConnectCallBack(conncb);
    connptr->setCloseCallBack(closecb);
    connptr->setMessageArrivedCallBack(macb);
    connptr->connectEstablished();
    running = true;
    while (running) {
        int cnt = epoll_wait(epfd, evpool, CLI_EVS, -1);
        if (cnt == -1) {
            if (errno == EINTR) {
                // Has been in shutdown state
                INFO("Signal coming: epoll_wait exits.");
            } else {
                ERROR(std::strerror(errno));
                break;
            }
        }
        for (int i = 0; i < cnt; ++i) {
            auto tg = reinterpret_cast<Trigger*>(evpool[i].data.ptr);
            tg->setRevents(evpool[i].events);
            tg->handleEvent();
        }
    }
}
