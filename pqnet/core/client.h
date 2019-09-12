#ifndef PQNET_CORE_CLIENT_H
#define PQNET_CORE_CLIENT_H

#include <cstdint>
#include <memory>

#include <sys/epoll.h>

#include "callback.h"
#include "connection.h"
#include "eventloop.h"
#include "ipaddr.h"
#include "trigger.h"

#include "../util/types.h"

namespace pqnet
{

class TcpClient
{
public:
    TcpClient(const char *servname, std::uint16_t port);
    ~TcpClient();
    int getLoopFd() const { return m_looper->getFd(); }
    void buildConn();
    TcpConnPtr getConn() const { return conn; }
    void start();
    void shutdown() { m_looper->quit(); }
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
    std::unique_ptr<EventLoop> m_looper;
};

} // namespace pqnet

#endif // PQNET_CORE_CLIENT_H
