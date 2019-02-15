#ifndef PQNET_CORE_SERVER_H
#define PQNET_CORE_SERVER_H

#include <cstdint>

#include <functional>

#include <sys/epoll.h>

#include "../util/types.h"

namespace pqnet
{

class TcpServer
{
public:
    TcpServer(std::uint16_t port);
    TcpServer(const char *servname, std::uint16_t port);
    ~TcpServer();
    void listen();
    void run(bool& done);
    void shutdown();
    void setConnectCallBack(connectCallBack cb) { conncb = cb; }
    static void onConn(TcpServer *servptr, int connfd);
    void onConnect(TcpServer *servptr, int connfd) { conncb(servptr, connfd); }
private:
    bool listening;
    bool running;
    int listenfd;
    connectCallBack conncb;
    int epfd;
    struct epoll_event poi;
    struct epoll_event evpool[MAXEVENTS];
};

} // namespace pqnet

#endif // PQNET_CORE_SERVER_H
