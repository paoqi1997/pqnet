#include <cerrno>
#include <cstring>

#include <netinet/in.h>
#include <sys/socket.h>

#include "../util/logger.h"
#include "client.h"
#include "socket.h"

using namespace pqnet;

TcpClient::TcpClient(const char *servname, std::uint16_t port)
    : addr(servname, port), looper(new EventLoop())
{
    this->init();
}

TcpClient::~TcpClient()
{
    if (conn->isConnected()) {
        conn->connectDestroyed();
    }
}

void TcpClient::start()
{
    setCallBacks();
    looper->loop();
}

void TcpClient::init()
{
    // socket
    int sockfd = new_socket();
    if (sockfd == -1) {
        ERROR(std::strerror(errno));
    }
    // connect
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
    conn = std::make_shared<TcpConnection>(looper.get(), sockfd);
}

void TcpClient::setCallBacks()
{
    conn->setConnectCallBack(conncb);
    conn->setCloseCallBack(closecb);
    conn->setMessageArrivedCallBack(macb);
    conn->setWriteCompletedCallBack(wccb);
    conn->connectEstablished();
}
