#include <cassert>
#include <cerrno>
#include <cstring>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../util/logger.h"
#include "ipaddr.h"
#include "server.h"
#include "socket.h"

using namespace pqnet;

TcpServer::TcpServer(std::uint16_t port) : index(0), ln(4), pool(ln)
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
    auto addrptr = reinterpret_cast<struct sockaddr*>(ipaddr.getAddr());
    if (bind(listenfd, addrptr, sizeof(struct sockaddr)) == -1) {
        ERROR(std::strerror(errno));
    }
    if (listen(listenfd, BACKLOG) == -1) {
        ERROR(std::strerror(errno));
    }
}

TcpServer::TcpServer(const char *servname, std::uint16_t port) : index(0), ln(4), pool(ln)
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
    auto addrptr = reinterpret_cast<struct sockaddr*>(ipaddr.getAddr());
    if (bind(listenfd, addrptr, sizeof(struct sockaddr)) == -1) {
        ERROR(std::strerror(errno));
    }
    if (listen(listenfd, BACKLOG) == -1) {
        ERROR(std::strerror(errno));
    }
}

TcpServer::~TcpServer()
{

}

void TcpServer::run()
{
    this->checkCallBack();
    pool.run();
    epfd = epoll_create(SERV_EVS);
    if (epfd == -1) {
        ERROR(std::strerror(errno));
    }
    poi.data.fd = listenfd;
    poi.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &poi) == -1) {
        ERROR(std::strerror(errno));
    }
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(struct sockaddr);
    auto addrptr = reinterpret_cast<struct sockaddr*>(&cliaddr);
    for ( ; ; ) {
        int cnt = epoll_wait(epfd, evpool, SERV_EVS, -1);
        if (cnt == -1) {
            if (errno == EINTR) {
                INFO("Signal coming: epoll_wait exits.");
                break;
            } else {
                ERROR(std::strerror(errno));
                continue;
            }
        }
        for (int i = 0; i < cnt; ++i) {
            if (evpool[i].data.fd == listenfd) {
                int connfd = accept(listenfd, addrptr, &clilen);
                if (connfd == -1) {
                    ERROR(std::strerror(errno));
                    continue;
                }
                this->onConnect(connfd);
            }
        }
    }
    this->shutdown();
}

void TcpServer::shutdown()
{
    for (std::size_t i = 0; i < ln; ++i) {
        int evfd = pool.pool[i]->evfd;
        std::uint64_t msg = 2;
        if (write(evfd, &msg, sizeof(std::uint64_t)) == -1) {
            ERROR(std::strerror(errno));
        }
    }
    pool.shutdown();
}

void TcpServer::checkCallBack()
{
    assert(conncb != nullptr);
    assert(closecb != nullptr);
    assert(readcb != nullptr);
    assert(msgcb != nullptr);
    pool.setConnectCallBack(conncb);
    pool.setCloseCallBack(closecb);
    pool.setReadCallBack(readcb);
    pool.setMessageCallBack(msgcb);
}

void TcpServer::onConnect(int connfd)
{
    setNonBlock(connfd, true);
    std::size_t loopindex = getNextLoopIndex();
    int evfd = pool.pool[loopindex]->evfd;
    std::uint64_t msg = EV_CONN;
    pool.pool[loopindex]->waitconns.push(connfd);
    if (write(evfd, &msg, sizeof(std::uint64_t)) == -1) {
        ERROR(std::strerror(errno));
    }
    std::printf("CONNECT!\n");
}

std::size_t TcpServer::getNextLoopIndex()
{
    if (index < ln) {
        return index++;
    } else {
        index = 0;
        return index;
    }
}
