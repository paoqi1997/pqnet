#include <cerrno>
#include <cstdio>
#include <cstring>

#include <iostream>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../util/logger.h"
#include "client.h"

using namespace pqnet;

TcpEchoClient::TcpEchoClient(const char *servname, std::uint16_t port) : addr(servname, port)
{
    sockfd = new_socket();
    if (sockfd == -1) {
        ERROR(std::strerror(errno));
    }
    auto addrptr = reinterpret_cast<struct sockaddr*>(addr.getAddrPtr());
    if (connect(sockfd, addrptr, sizeof(struct sockaddr)) == -1) {
        ERROR(std::strerror(errno));
    }
    epfd = epoll_create(8);
    if (epfd == -1) {
        ERROR(std::strerror(errno));
    }
    poi.events = EPOLLIN;
    poi.data.fd = sockfd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &poi) == -1) {
        ERROR(std::strerror(errno));
    }
    poi.events = EPOLLIN;
    poi.data.fd = fileno(stdin);
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fileno(stdin), &poi) == -1) {
        ERROR(std::strerror(errno));
    }
}

TcpEchoClient::~TcpEchoClient()
{

}

void TcpEchoClient::run()
{
    for ( ; ; ) {
        int cnt = epoll_wait(epfd, evpool, 8, -1);
        if (cnt == -1) {
            if (errno == EINTR) {
                break;
            } else {
                ERROR(std::strerror(errno));
                continue;
            }
        }
        if (cnt > 0) {
            for (int i = 0; i < cnt; ++i) {
                if (evpool[i].data.fd == sockfd) {
                    buffer.readFrom(sockfd, buffer.writableBytes());
                    buffer.writeTo(fileno(stdout), buffer.readableBytes());
                }
                if (evpool[i].data.fd == fileno(stdin)) {
                    std::cin >> msg;
                    buffer.append(msg.c_str(), msg.size());
                    buffer.appendInt8('\n');
                    buffer.writeTo(sockfd, buffer.readableBytes());
                }
            }
        }
    }
    this->shutdown();
}

void TcpEchoClient::shutdown()
{
    if (close(sockfd) == -1) {
        ERROR(std::strerror(errno));
    }
    if (close(epfd) == -1) {
        ERROR(std::strerror(errno));
    }
}
