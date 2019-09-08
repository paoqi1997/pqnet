#ifndef PQNET_CORE_SERVER_H
#define PQNET_CORE_SERVER_H

#include <cstdint>
#include <map>
#include <memory>

#include <sys/epoll.h>

#include "callback.h"
#include "connection.h"
#include "eventloop.h"
#include "eventloopthreadpool.h"
#include "ipaddr.h"
#include "trigger.h"

#include "../util/types.h"

namespace pqnet
{

class TcpServer
{
public:
    TcpServer(std::uint16_t port);
    TcpServer(const char *servname, std::uint16_t port);
    ~TcpServer();
    void run();
    void shutdown() { m_looper->shutdown(); }
    void setConnectCallBack(const connectCallBack& cb) { conncb = cb; }
    void setCloseCallBack(const closeCallBack& cb) { closecb = cb; }
    void setMessageArrivedCallBack(const messageArrivedCallBack& cb) { macb = cb; }
    void setWriteCompletedCallBack(const writeCompletedCallBack& cb) { wccb = cb; }
private:
    void onAccept();
    void CloseUp();
private:
    connectCallBack conncb;
    closeCallBack closecb;
    messageArrivedCallBack macb;
    writeCompletedCallBack wccb;
    int listenfd;
    Ip4Addr addr;
    std::map<int, TcpConnPtr> connpool;
    std::unique_ptr<Trigger> listener;
    std::unique_ptr<EventLoop> m_looper;
    std::unique_ptr<EventLoopThreadPool> m_pool;
};

} // namespace pqnet

#endif // PQNET_CORE_SERVER_H
