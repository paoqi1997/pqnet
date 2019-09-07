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
    : addr(servname, port), m_looper(new EventLoop())
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
    if (connect(sockfd, addrptr, sizeof(struct sockaddr)) == -1) {
        ERROR(std::strerror(errno));
    }
    setNonBlock(sockfd, true);
    setReuseAddr(sockfd, true);
    setReusePort(sockfd, true);
    conn = std::make_shared<TcpConnection>(m_looper->getFd(), sockfd);
    conn->setConnectCallBack(conncb);
    conn->setCloseCallBack(closecb);
    conn->setMessageArrivedCallBack(macb);
    conn->setWriteCompletedCallBack(wccb);
    conn->connectEstablished();
}

void TcpClient::start()
{
    buildConn();
    m_looper->loop();
}
