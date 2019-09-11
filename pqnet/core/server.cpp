#include "server.h"

#include <cassert>
#include <cerrno>
#include <cstring>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "socket.h"
#include "../util/logger.h"

using namespace pqnet;

TcpServer::TcpServer(std::uint16_t port)
    : addr(port), listenTrigger(new Trigger()),
      leader(new EventLoop()), followers(new EventLoopThreadPool(2))
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
    auto addrptr = reinterpret_cast<struct sockaddr*>(addr.getPtr());
    if (bind(listenfd, addrptr, sizeof(struct sockaddr)) == -1) {
        ERROR(std::strerror(errno));
    }
    // listen
    if (listen(listenfd, BACKLOG) == -1) {
        ERROR(std::strerror(errno));
    }
}

TcpServer::TcpServer(const char *servname, std::uint16_t port)
    : addr(servname, port), listenTrigger(new Trigger()),
      leader(new EventLoop()), followers(new EventLoopThreadPool(2))
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
    auto addrptr = reinterpret_cast<struct sockaddr*>(addr.getPtr());
    if (bind(listenfd, addrptr, sizeof(struct sockaddr)) == -1) {
        ERROR(std::strerror(errno));
    }
    // listen
    if (listen(listenfd, BACKLOG) == -1) {
        ERROR(std::strerror(errno));
    }
}

TcpServer::~TcpServer()
{
    if (close(listenfd) == -1) {
        ERROR(std::strerror(errno));
    }
    this->clear();
}

void TcpServer::start()
{
    listenTrigger->setFds(leader->getFd(), listenfd);
    listenTrigger->setReadHandler(std::bind(&TcpServer::onAccept, this));
    listenTrigger->addToLoop();
    listenTrigger->likeReading();
    followers->start();
    leader->loop();
}

void TcpServer::onAccept()
{
    TRACE("Fd: %d, Func: TcpServer::%s", leader->getFd(), __func__);
    struct sockaddr_in cliaddr;
    auto addrptr = reinterpret_cast<struct sockaddr*>(&cliaddr);
    socklen_t clilen = sizeof(struct sockaddr);
    int connfd = accept(listenfd, addrptr, &clilen);
    if (connfd == -1) {
        ERROR(std::strerror(errno));
    }
    setNonBlock(connfd, true);
    auto currLooper = followers->getNextLoop();
    connpool[connfd] = std::make_shared<TcpConnection>(currLooper->getFd(), connfd);
    connpool[connfd]->setConnectCallBack(conncb);
    connpool[connfd]->setCloseCallBack(closecb);
    connpool[connfd]->setMessageArrivedCallBack(macb);
    connpool[connfd]->setWriteCompletedCallBack(wccb);
    currLooper->pushFn(std::bind(&TcpConnection::connectEstablished, connpool[connfd]));
    this->notify(currLooper->getEvfd(), EV_CONN);
    TRACE("Connection %d in Looper %d.", connfd, currLooper->getFd());
}

void TcpServer::notify(int evfd, std::uint64_t msg)
{
    if (write(evfd, &msg, sizeof(std::uint64_t)) == -1) {
        ERROR(std::strerror(errno));
    }
}

void TcpServer::clear()
{
    for (std::size_t i = 0; i < followers->size(); ++i) {
        int evfd = followers->getEvfdByIndex(i);
        std::uint64_t msg = EV_EXIT;
        if (write(evfd, &msg, sizeof(std::uint64_t)) == -1) {
            ERROR(std::strerror(errno));
        }
    }
}
