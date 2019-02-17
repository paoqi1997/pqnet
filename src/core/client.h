#ifndef PQNET_CORE_CLIENT_H
#define PQNET_CORE_CLIENT_H

#include <cstdint>

#include <string>

#include <sys/epoll.h>

#include "buffer.h"
#include "ipaddr.h"
#include "socket.h"

namespace pqnet
{

class TcpEchoClient
{
public:
    TcpEchoClient(const char *servname, std::uint16_t port);
    ~TcpEchoClient();
    void run();
    void shutdown();
private:
    int sockfd;
    Ip4Addr addr;
    Buffer buffer;
    std::string msg;
    int epfd;
    struct epoll_event poi;
    struct epoll_event evpool[8];
};

} // namespace pqnet

#endif // PQNET_CORE_CLIENT_H
