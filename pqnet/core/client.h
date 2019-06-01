#ifndef PQNET_CORE_CLIENT_H
#define PQNET_CORE_CLIENT_H

#include <cstdint>
#include <vector>

#include <sys/epoll.h>

#include "../util/types.h"
#include "callback.h"
#include "channel.h"
#include "connection.h"
#include "ipaddr.h"

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
    // for stdin
    void setInCallBack(const inCallBack& cb) { incb = cb; }
    // CallBack(s)
    void setConnectCallBack(const connectCallBack& cb) { conncb = cb; }
    void setReadCallBack(const readCallBack& cb) { readcb = cb; }
    void setMessageCallBack(const messageCallBack& cb) { msgcb = cb; }
    void setCloseByPeerCallBack(const closeByPeerCallBack& cb) { cpcb = cb; }
    void setCloseBySockCallBack(const closeBySockCallBack& cb) { cscb = cb; }
private:
    void checkCallBack();
private:
    inCallBack incb;
    connectCallBack conncb;
    readCallBack readcb;
    messageCallBack msgcb;
    closeByPeerCallBack cpcb;
    closeBySockCallBack cscb;
    int sockfd;
    Ip4Addr addr;
    TcpConnPtr connptr;
    std::vector<Channel*> channels;
    bool running;
    int epfd;
    struct epoll_event poi;
    struct epoll_event evpool[CLI_EVS];
};

} // namespace pqnet

#endif // PQNET_CORE_CLIENT_H
