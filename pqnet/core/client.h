#ifndef PQNET_CORE_CLIENT_H
#define PQNET_CORE_CLIENT_H

#include <cstdint>

#include <sys/epoll.h>

#include "../util/types.h"
#include "callback.h"
#include "connection.h"
#include "ipaddr.h"
#include "trigger.h"

namespace pqnet
{

class TcpClient
{
public:
    TcpClient(const char *servname, std::uint16_t port);
    ~TcpClient();
    void run();
    void shutdown() { running = false; }
    int getEpfd() const { return epfd; }
    TcpConnPtr getConn() const { return connptr; }
    void setConnectCallBack(const connectCallBack& cb) { conncb = cb; }
    void setCloseCallBack(const closeCallBack& cb) { closecb = cb; }
    void setMessageArrivedCallBack(const messageArrivedCallBack& cb) { macb = cb; }
    void setWriteCompletedCallBack(const writeCompletedCallBack& cb) { wccb = cb; }
private:
    connectCallBack conncb;
    closeCallBack closecb;
    messageArrivedCallBack macb;
    writeCompletedCallBack wccb;
    int sockfd;
    Ip4Addr addr;
    TcpConnPtr connptr;
    bool running;
    int epfd;
    struct epoll_event poi;
    struct epoll_event evpool[CLI_EVS];
};

} // namespace pqnet

#endif // PQNET_CORE_CLIENT_H
