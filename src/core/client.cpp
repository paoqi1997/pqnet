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
    : addr(servname, port), running(false), endmsg(_endmsg)
{
    // socket
    sockfd = new_socket();
    if (sockfd == -1) {
        ERROR(std::strerror(errno));
    }
    // connect
    auto addrptr = reinterpret_cast<struct sockaddr*>(addr.getAddr());
    if (connect(sockfd, addrptr, sizeof(struct sockaddr)) == -1) {
        ERROR(std::strerror(errno));
    }
    epfd = epoll_create(CLI_EVS);
    if (epfd == -1) {
        ERROR(std::strerror(errno));
    }
    poi.events = EPOLLRDHUP | EPOLLIN;
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

// close(Client) -> EPOLLRDHUP(Looper)
TcpEchoClient::~TcpEchoClient()
{
    if (close(epfd) == -1) {
        ERROR(std::strerror(errno));
    }
    // 关闭连接并告知服务端
    if (close(sockfd) == -1) {
        ERROR(std::strerror(errno));
    }
}

void TcpEchoClient::run()
{
    running = true;
    while (running) {
        int cnt = epoll_wait(epfd, evpool, CLI_EVS, -1);
        if (cnt == -1) {
            if (errno == EINTR) {
                INFO("Signal coming: epoll_wait exits.");
            } else {
                ERROR(std::strerror(errno));
                break;
            }
        }
        for (int i = 0; i < cnt; ++i) {
            // 服务端关闭连接
            if (evpool[i].events & EPOLLRDHUP) {
                this->shutdown();
            }
            else if (evpool[i].events & EPOLLIN) {
                if (evpool[i].data.fd == sockfd) {
                    this->recv();
                    msg = this->get();
                    if (msg == endmsg) {
                        this->shutdown();
                    } else {
                        std::cout << msg;
                    }
                }
                if (evpool[i].data.fd == fileno(stdin)) {
                    std::cin >> msg; msg += '\n';
                    this->append(msg.c_str());
                    this->send();
                }
            }
        }
    }
}
