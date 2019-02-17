#include <cerrno>
#include <cstdio>
#include <cstring>

#include <iostream>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../util/logger.h"
#include "client.h"
#include "socket.h"

using namespace pqnet;

TcpEchoClient::TcpEchoClient(const char *servname, std::uint16_t port, std::string _endmsg)
    : addr(servname, port), endmsg(_endmsg)
{
    sockfd = new_socket();
    if (sockfd == -1) {
        ERROR(std::strerror(errno));
    }
    auto addrptr = reinterpret_cast<struct sockaddr*>(addr.getAddr());
    if (connect(sockfd, addrptr, sizeof(struct sockaddr)) == -1) {
        ERROR(std::strerror(errno));
    }
    epfd = epoll_create(CLI_EVS);
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
        int cnt = epoll_wait(epfd, evpool, CLI_EVS, -1);
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
                    msg = buffer.get(buffer.readableBytes());
                    std::cout << msg;
                    if (msg == endmsg) {
                        break;
                    }
                }
                if (evpool[i].data.fd == fileno(stdin)) {
                    std::cin >> msg; msg += '\n';
                    buffer.append(msg.c_str(), msg.size());
                    buffer.writeTo(sockfd, buffer.readableBytes());
                }
            }
        }
        if (msg == endmsg) {
            break;
        }
    }
    this->shutdown();
}

void TcpEchoClient::shutdown()
{
    if (close(epfd) == -1) {
        ERROR(std::strerror(errno));
    }
    if (close(sockfd) == -1) {
        ERROR(std::strerror(errno));
    }
}
