#include "server.h"

#include <cerrno>
#include <cstring>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "socket.h"
#include "../util/logger.h"
#include "../util/types.h"

using namespace pqnet;
using namespace std::placeholders;

TcpServer::TcpServer(std::uint16_t port)
    : addr(port), listenTrigger(new Trigger()),
      leader(new EventLoop()), followers(new EventLoopThreadPool(2))
{
    // socket
    listenfd = new_socket();
    if (listenfd == -1) {
        ERROR(std::strerror(errno));
    }
    if (!setNonBlock(listenfd, true)) {
        ERROR(std::strerror(errno));
    }
    if (!setReuseAddr(listenfd, true)) {
        ERROR(std::strerror(errno));
    }
    if (!setReusePort(listenfd, true)) {
        ERROR(std::strerror(errno));
    }
    // bind
    auto addrptr = reinterpret_cast<struct sockaddr*>(addr.getPtr());
    if (bind(listenfd, addrptr, sizeof(struct sockaddr_in)) == -1) {
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
    if (!setNonBlock(listenfd, true)) {
        ERROR(std::strerror(errno));
    }
    if (!setReuseAddr(listenfd, true)) {
        ERROR(std::strerror(errno));
    }
    if (!setReusePort(listenfd, true)) {
        ERROR(std::strerror(errno));
    }
    // bind
    auto addrptr = reinterpret_cast<struct sockaddr*>(addr.getPtr());
    if (bind(listenfd, addrptr, sizeof(struct sockaddr_in)) == -1) {
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
    for (auto& kv : connpool) {
        // TODO: Destroy connections by followers
        kv.second->connectDestroyed();
    }
    this->clear();
}

void TcpServer::start()
{
    listenTrigger->setEpfd(leader->getFd());
    listenTrigger->setFd(listenfd);
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
    socklen_t clilen = sizeof(struct sockaddr_in);
    int connfd = accept(listenfd, addrptr, &clilen);
    if (connfd == -1) {
        ERROR(std::strerror(errno));
    }
    if (!setNonBlock(connfd, true)) {
        ERROR(std::strerror(errno));
    }
    auto currLooper = followers->getNextEventLoop();
    connpool[connfd] = std::make_shared<TcpConnection>(currLooper, connfd);
    connpool[connfd]->setConnectCallBack(conncb);
    connpool[connfd]->setCloseCallBack(closecb);
    connpool[connfd]->setImplCloseCallBack(std::bind(&TcpServer::removeConnection, this, _1));
    connpool[connfd]->setMessageArrivedCallBack(macb);
    connpool[connfd]->setWriteCompletedCallBack(wccb);
    currLooper->pushFn(std::bind(&TcpConnection::connectEstablished, connpool[connfd]));
    this->tell(currLooper->getEvfd(), EV_CONN);
    TRACE("Connection %d in Looper %d.", connfd, currLooper->getFd());
}

void TcpServer::clear()
{
    for (std::size_t i = 0; i < followers->size(); ++i) {
        int evfd = followers->getEventLoopByIndex(i)->getEvfd();
        this->tell(evfd, EV_EXIT);
    }
}

void TcpServer::tell(int evfd, std::uint64_t msg)
{
    if (write(evfd, &msg, sizeof(std::uint64_t)) == -1) {
        ERROR(std::strerror(errno));
    }
}
