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
    int getEpfd() const { return epfd; }
    void buildConn();
    TcpConnPtr getConn() const { return conn; }
    void run();
    void shutdown() { running = false; }
    void setConnectCallBack(const connectCallBack& cb) { conncb = cb; }
    void setCloseCallBack(const closeCallBack& cb) { closecb = cb; }
    void setMessageArrivedCallBack(const messageArrivedCallBack& cb) { macb = cb; }
    void setWriteCompletedCallBack(const writeCompletedCallBack& cb) { wccb = cb; }
private:
    connectCallBack conncb;
    closeCallBack closecb;
    messageArrivedCallBack macb;
    writeCompletedCallBack wccb;
    Ip4Addr addr;
    TcpConnPtr conn;
    bool running;
    int epfd;
    struct epoll_event evpool[EPOLLSIZE];
};

} // namespace pqnet

#endif // PQNET_CORE_CLIENT_H
