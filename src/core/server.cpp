#include <cerrno>
#include <cstring>

#include <netinet/in.h>
#include <sys/socket.h>

#include "../util/logger.h"
#include "ipaddr.h"
#include "server.h"
#include "socket.h"

using namespace pqnet;

TcpServer::TcpServer(std::uint16_t port) : listening(false), running(false), conncb(nullptr)
{
    // socket
    listenfd = new_socket();
    if (listenfd == -1) {
        ERROR(std::strerror(errno));
    }
    setNonBlock(listenfd, true);
    setReuseAddr(listenfd, true);
    setReusePort(listenfd, true);
    // bind
    Ip4Addr ipaddr(port);
    auto addrptr = static_cast<struct sockaddr*>(ipaddr.getAddrPtr());
    if (bind(listenfd, addrptr, sizeof(struct sockaddr)) == -1) {
        ERROR(std::strerror(errno));
    }
}

TcpServer::TcpServer(const char *servname, std::uint16_t port)
    : listening(false), running(false), conncb(nullptr)
{
    // socket
    listenfd = new_socket();
    if (listenfd == -1) {
        ERROR(std::strerror(errno));
    }
    setNonBlock(listenfd, true);
    setReuseAddr(listenfd, true);
    setReusePort(listenfd, true);
    // bind
    Ip4Addr ipaddr(servname, port);
    auto addrptr = static_cast<struct sockaddr*>(ipaddr.getAddrPtr());
    if (bind(listenfd, addrptr, sizeof(struct sockaddr)) == -1) {
        ERROR(std::strerror(errno));
    }
}

TcpServer::~TcpServer()
{

}

void TcpServer::listen()
{
    if (listen(listenfd, BACKLOG) == -1) {
        ERROR(std::strerror(errno));
    }
    listening = true;
}

void TcpServer::run(bool& done)
{
    if (!listening) {
        this->listen();
    }
    if (conncb == nullptr) {
        conncb = onConn;
    }
    epfd = epoll_create(MAXEVENTS);
    if (epfd == -1) {
        ERROR(std::strerror(errno));
    }
    poi.data.fd = listenfd;
    poi.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &poi) == -1) {
        ERROR(std::strerror(errno));
    }
    struct sockaddr_in cliaddr;
    auto addrptr = static_cast<struct sockaddr*>(cliaddr);
    while (!done) {
        int cnt = epoll_wait(epfd, evpool, MAXEVENTS, -1);
        if (cnt == -1) {
            ERROR(std::strerror(errno));
            break;
        }
        for (int i = 0; i < cnt; ++i) {
            if (evpool[i].data.fd == listenfd) {
                int connfd = accept(listenfd, addrptr, sizeof(struct sockaddr));
                if (connfd == -1) {
                    ERROR(std::strerror(errno));
                    continue;
                }
                setNonBlock(connfd, true);
                this->onConnect(this, connfd);
            }
        }
    }
    this->shutdown();
}

void TcpServer::shutdown()
{

}

void TcpServer::onConn(TcpServer *servptr, int connfd)
{

}
