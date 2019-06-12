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
    epfd = epoll_create(CLI_EVS);
    if (epfd == -1) {
        ERROR(std::strerror(errno));
    }
}

TcpClient::~TcpClient()
{
    if (conn->isConnected()) {
        conn->connectDestroyed();
    }
    if (close(epfd) == -1) {
        ERROR(std::strerror(errno));
    }
}

void TcpClient::buildConn()
{
    int sockfd = new_socket();
    if (sockfd == -1) {
        ERROR(std::strerror(errno));
    }
    auto addrptr = reinterpret_cast<struct sockaddr*>(addr.getPtr());
    if (connect(sockfd, addrptr, sizeof(struct sockaddr)) == -1) {
        ERROR(std::strerror(errno));
    }
    setNonBlock(sockfd, true);
    setReuseAddr(sockfd, true);
    setReusePort(sockfd, true);
    conn = std::make_shared<TcpConnection>(epfd, sockfd);
    conn->setConnectCallBack(conncb);
    conn->setCloseCallBack(closecb);
    conn->setMessageArrivedCallBack(macb);
    conn->setWriteCompletedCallBack(wccb);
    conn->connectEstablished();
}

void TcpClient::run()
{
    this->buildConn();
    running = true;
    while (running) {
        int cnt = epoll_wait(epfd, evpool, CLI_EVS, -1);
        if (cnt == -1) {
            if (errno == EINTR) {
                TRACE("epoll_wait is interrupted by a signal  handler.");
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
