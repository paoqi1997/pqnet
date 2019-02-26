#ifndef PQNET_CORE_CLIENT_H
#define PQNET_CORE_CLIENT_H

#include <cstdint>

#include <map>
#include <string>

#include <sys/epoll.h>

#include "../util/types.h"
#include "callback.h"
#include "connection.h"
#include "ipaddr.h"

namespace pqnet
{

class TcpEchoClient
{
public:
    TcpEchoClient(const char *servname, std::uint16_t port, std::string _endmsg = "quit\n");
    ~TcpEchoClient();
    void run();
    void shutdown() { running = false; }
private:
    int sockfd;
    Ip4Addr addr;
    TcpCliConnPtr connptr;
    bool running;
    std::string msg;
    std::string endmsg;
    std::map<int, TcpCliConnPtr> connpool;
    int epfd;
    struct epoll_event poi;
    struct epoll_event evpool[CLI_EVS];
};

} // namespace pqnet

#endif // PQNET_CORE_CLIENT_H
