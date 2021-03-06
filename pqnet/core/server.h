#ifndef PQNET_CORE_SERVER_H
#define PQNET_CORE_SERVER_H

#include <cstdint>
#include <map>
#include <memory>

#include "callback.h"
#include "connection.h"
#include "eventloop.h"
#include "eventloopthreadpool.h"
#include "ipaddr.h"
#include "trigger.h"

namespace pqnet
{

class TcpServer
{
public:
    TcpServer(std::uint16_t port);
    TcpServer(const char *servname, std::uint16_t port);
    ~TcpServer();
    void start();
    void shutdown() { leader->quit(); }
    void setConnectCallBack(const connectCallBack& cb) { conncb = cb; }
    void setCloseCallBack(const closeCallBack& cb) { closecb = cb; }
    void setMessageArrivedCallBack(const messageArrivedCallBack& cb) { macb = cb; }
    void setWriteCompletedCallBack(const writeCompletedCallBack& cb) { wccb = cb; }
private:
    void init();
    void onAccept();
    void clearFollowers();
    void removeConnection(const TcpConnPtr& conn);
    void removeConnectionInLoop(const TcpConnPtr& conn);
private:
    connectCallBack conncb;
    closeCallBack closecb;
    messageArrivedCallBack macb;
    writeCompletedCallBack wccb;
    Ip4Addr addr;
    std::map<int, TcpConnPtr> connpool;
    std::unique_ptr<Trigger> listenTrigger;
    std::unique_ptr<EventLoop> leader;
    std::unique_ptr<EventLoopThreadPool> followers;
};

} // namespace pqnet

#endif // PQNET_CORE_SERVER_H
