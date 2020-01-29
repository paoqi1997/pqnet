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
    : addr(servname, port), looper(new EventLoop())
{

}

TcpClient::~TcpClient()
{
    if (conn->isConnected()) {
        conn->connectDestroyed();
    }
}

void TcpClient::buildConn()
{
    int sockfd = new_socket();
    if (sockfd == -1) {
        ERROR(std::strerror(errno));
    }
    auto addrptr = reinterpret_cast<struct sockaddr*>(addr.getPtr());
    if (connect(sockfd, addrptr, sizeof(struct sockaddr_in)) == -1) {
        ERROR(std::strerror(errno));
    }
    if (!setNonBlock(sockfd, true)) {
        ERROR(std::strerror(errno));
    }
    if (!setReuseAddr(sockfd, true)) {
        ERROR(std::strerror(errno));
    }
    if (!setReusePort(sockfd, true)) {
        ERROR(std::strerror(errno));
    }
    conn = std::make_shared<TcpConnection>(looper->getFd(), sockfd);
    conn->setConnectCallBack(conncb);
    conn->setCloseCallBack(closecb);
    conn->setMessageArrivedCallBack(macb);
    conn->setWriteCompletedCallBack(wccb);
    conn->connectEstablished();
}

void TcpClient::start()
{
    buildConn();
    looper->loop();
}
