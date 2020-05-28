#include <cerrno>
#include <cstring>
#include <fstream>
#include <string>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../util/logger.h"
#include "server.h"
#include "socket.h"

using namespace pqnet;
using namespace std::placeholders;

namespace {
std::size_t nProcessors = 0;
} // unnamed namespace

std::size_t getNumberOfProcessors()
{
    if (nProcessors == 0) {
        const char *sFileName = "/proc/cpuinfo";
        std::ifstream is(sFileName);
        if (!is.is_open()) {
            ERROR("Failed to open %s.\n", sFileName);
            nProcessors = 1;
        } else {
            std::string line;
            while (!is.eof()) {
                std::getline(is, line);
                if (line.find("processor") != std::string::npos) {
                    ++nProcessors;
                }
            }
        }
        if (is.is_open()) {
            is.close();
        }
    }
    return nProcessors;
}

TcpServer::TcpServer(std::uint16_t port)
    : addr(port), listenTrigger(new Trigger()),
      leader(new EventLoop()), followers(new EventLoopThreadPool(getNumberOfProcessors() * 2))
{
    this->init();
}

TcpServer::TcpServer(const char *servname, std::uint16_t port)
    : addr(servname, port), listenTrigger(new Trigger()),
      leader(new EventLoop()), followers(new EventLoopThreadPool(getNumberOfProcessors() * 2))
{
    this->init();
}

TcpServer::~TcpServer()
{
    listenTrigger->removeFromLoop();
    if (close(listenTrigger->getFd()) == -1) {
        ERROR(std::strerror(errno));
    }
    for (auto& kv : connpool) {
        auto looper = kv.second->getEventLoop();
        looper->pushFunctor(std::bind(&TcpConnection::connectDestroyed, kv.second));
    }
    this->clearFollowers();
}

void TcpServer::start()
{
    followers->start();
    leader->loop();
}

void TcpServer::init()
{
    // socket
    int listenfd = new_socket();
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
    DEBUG("ListenFd: %d, Listen to %s", listenfd, addr.toString().c_str());
    // For ListenFd
    listenTrigger->setEpfd(leader->getFd());
    listenTrigger->setFd(listenfd);
    listenTrigger->setReadHandler(std::bind(&TcpServer::onAccept, this));
    listenTrigger->addToLoop();
    listenTrigger->likeReading();
}

void TcpServer::onAccept()
{
    DEBUG("LooperFd: %d, Func: TcpServer::%s", leader->getFd(), __func__);
    struct sockaddr_in cliaddr;
    auto addrptr = reinterpret_cast<struct sockaddr*>(&cliaddr);
    socklen_t clilen = sizeof(struct sockaddr_in);
    int connfd;
    while ((connfd = accept(listenTrigger->getFd(), addrptr, &clilen)) != -1) {
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
        currLooper->pushFunctor(std::bind(&TcpConnection::connectEstablished, connpool[connfd]));
        DEBUG("Connection %d in Looper %d.", connfd, currLooper->getFd());
    }
    if (errno == EAGAIN) {
        DEBUG(MSG_ACCEPT_EAGAIN);
    } else {
        ERROR(std::strerror(errno));
    }
}

void TcpServer::clearFollowers()
{
    for (std::size_t i = 0; i < followers->size(); ++i) {
        auto looper = followers->getEventLoopByIndex(i);
        looper->pushFunctor(std::bind(&EventLoop::quit, looper));
    }
}
