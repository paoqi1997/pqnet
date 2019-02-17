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

TcpServer::TcpServer(std::uint16_t port, bool _done)
    : listening(false), done(_done), ln(4), pool(ln), index(0)
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
    auto addrptr = reinterpret_cast<struct sockaddr*>(ipaddr.getAddrPtr());
    if (bind(listenfd, addrptr, sizeof(struct sockaddr)) == -1) {
        ERROR(std::strerror(errno));
    }
    if (listen(listenfd, BACKLOG) == -1) {
        ERROR(std::strerror(errno));
    }
}

TcpServer::TcpServer(const char *servname, std::uint16_t port, bool _done)
    : listening(false), done(_done), ln(4), pool(ln), index(0)
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
    auto addrptr = reinterpret_cast<struct sockaddr*>(ipaddr.getAddrPtr());
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
    socklen_t clilen = sizeof(cliaddr);
    auto addrptr = reinterpret_cast<struct sockaddr*>(&cliaddr);
    for ( ; ; ) {
        if (done) {
            break;
        }
        int cnt = epoll_wait(epfd, evpool, MAXEVENTS, -1);
        if (cnt == -1) {
            ERROR(std::strerror(errno));
            break;
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

}

void TcpServer::checkCallBack()
{
    /*
    auto ptr = std::make_shared<TcpConnection>(listenfd);
    conncb(ptr);
    */
    auto cb = [](const TcpConnPtr&){};
    if (conncb == nullptr) {
        std::printf("NoConn!\n");
        conncb = cb;
    }
    if (closecb == nullptr) {
        std::printf("NoClose!\n");
        closecb = cb;
    }
    if (readcb == nullptr) {
        std::printf("NoRead!\n");
        readcb = cb;
    }
    if (msgcb == nullptr) {
        std::printf("NoMsg!\n");
        msgcb = cb;
    }
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
    std::uint64_t msg = 1;
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
