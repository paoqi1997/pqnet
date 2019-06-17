#ifndef PQNET_CORE_SERVER_H
#define PQNET_CORE_SERVER_H

#include <cstdint>

#include <sys/epoll.h>

#include "../util/types.h"
#include "callback.h"
#include "ipaddr.h"
#include "looperpool.h"

namespace pqnet
{

class TcpServer
{
public:
    TcpServer(std::uint16_t port);
    TcpServer(const char *servname, std::uint16_t port);
    ~TcpServer();
    void run();
    void shutdown() { running = false; }
    void setConnectCallBack(const connectCallBack& cb) { conncb = cb; }
    void setCloseCallBack(const closeCallBack& cb) { closecb = cb; }
    void setMessageArrivedCallBack(const messageArrivedCallBack& cb) { macb = cb; }
    void setWriteCompletedCallBack(const writeCompletedCallBack& cb) { wccb = cb; }
private:
    void onConnect(int connfd);
    std::size_t getNextLoopIndex();
private:
    connectCallBack conncb;
    closeCallBack closecb;
    messageArrivedCallBack macb;
    writeCompletedCallBack wccb;
    std::size_t index;
    std::size_t ln;
    LooperPool pool;
    int listenfd;
    Ip4Addr addr;
    bool running;
    int epfd;
    struct epoll_event poi;
    struct epoll_event evpool[EPOLLSIZE];
};

} // namespace pqnet

#endif // PQNET_CORE_SERVER_H
