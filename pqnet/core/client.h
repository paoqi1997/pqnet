#ifndef PQNET_CORE_CLIENT_H
#define PQNET_CORE_CLIENT_H

#include <cstdint>
#include <memory>

#include "callback.h"
#include "connection.h"
#include "eventloop.h"
#include "ipaddr.h"
#include "trigger.h"

namespace pqnet
{

class TcpClient
{
public:
    TcpClient(const char *servname, std::uint16_t port);
    ~TcpClient();
    int getLooperFd() const { return looper->getFd(); }
    TcpConnPtr getConn() const { return conn; }
    void start();
    void shutdown() { looper->quit(); }
    void setConnectCallBack(const connectCallBack& cb) { conncb = cb; }
    void setCloseCallBack(const closeCallBack& cb) { closecb = cb; }
    void setMessageArrivedCallBack(const messageArrivedCallBack& cb) { macb = cb; }
    void setWriteCompletedCallBack(const writeCompletedCallBack& cb) { wccb = cb; }
private:
    void init();
    void setCallBacks();
private:
    connectCallBack conncb;
    closeCallBack closecb;
    messageArrivedCallBack macb;
    writeCompletedCallBack wccb;
    Ip4Addr addr;
    TcpConnPtr conn;
    std::unique_ptr<EventLoop> looper;
};

} // namespace pqnet

#endif // PQNET_CORE_CLIENT_H
