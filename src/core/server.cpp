#include <cassert>
#include <cerrno>
#include <cstring>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../util/logger.h"
#include "server.h"
#include "socket.h"

using namespace pqnet;

TcpServer::TcpServer(std::uint16_t port) : index(0), ln(4), pool(ln), addr(port), running(false)
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
    auto addrptr = reinterpret_cast<struct sockaddr*>(addr.getAddr());
    if (bind(listenfd, addrptr, sizeof(struct sockaddr)) == -1) {
        ERROR(std::strerror(errno));
    }
    // listen
    if (listen(listenfd, BACKLOG) == -1) {
        ERROR(std::strerror(errno));
    }
}

TcpServer::TcpServer(const char *servname, std::uint16_t port)
    : index(0), ln(4), pool(ln), addr(servname, port), running(false)
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
    auto addrptr = reinterpret_cast<struct sockaddr*>(addr.getAddr());
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

}

void TcpServer::run()
{
    this->checkCallBack();
    pool.run();
    epfd = epoll_create(SERV_EVS);
    if (epfd == -1) {
        ERROR(std::strerror(errno));
    }
    poi.events = EPOLLET | EPOLLIN;
    poi.data.fd = listenfd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &poi) == -1) {
        ERROR(std::strerror(errno));
    }
    running = true;
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(struct sockaddr);
    auto addrptr = reinterpret_cast<struct sockaddr*>(&cliaddr);
    while (running) {
        int cnt = epoll_wait(epfd, evpool, SERV_EVS, -1);
        if (cnt == -1) {
            if (errno == EINTR) {
                INFO("Signal coming: epoll_wait exits.");
            } else {
                ERROR(std::strerror(errno));
                break;
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
    if (close(epfd) == -1) {
        ERROR(std::strerror(errno));
    }
    if (close(listenfd) == -1) {
        ERROR(std::strerror(errno));
    }
    for (std::size_t i = 0; i < ln; ++i) {
        int evfd = pool.loopers[i]->evfd;
        std::uint64_t msg = EV_EXIT;
        if (write(evfd, &msg, sizeof(std::uint64_t)) == -1) {
            ERROR(std::strerror(errno));
        }
    }
    pool.shutdown();
}

void TcpServer::checkCallBack()
{
    assert(conncb != nullptr);
    assert(readcb != nullptr);
    assert(msgcb != nullptr);
    assert(cpcb != nullptr);
    assert(cscb != nullptr);
    pool.setConnectCallBack(conncb);
    pool.setReadCallBack(readcb);
    pool.setMessageCallBack(msgcb);
    pool.setCloseByPeerCallBack(cpcb);
    pool.setCloseBySockCallBack(cscb);
}

void TcpServer::onConnect(int connfd)
{
    setNonBlock(connfd, true);
    std::size_t li = this->getNextLoopIndex();
    int evfd = pool.loopers[li]->evfd;
    std::uint64_t msg = EV_CONN;
    pool.loopers[li]->waitconns.push(connfd);
    if (write(evfd, &msg, sizeof(std::uint64_t)) == -1) {
        ERROR(std::strerror(errno));
    }
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
