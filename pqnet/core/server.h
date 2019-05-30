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
    void setReadCallBack(const readCallBack& cb) { readcb = cb; }
    void setMessageCallBack(const messageCallBack& cb) { msgcb = cb; }
    void setCloseByPeerCallBack(const closeByPeerCallBack& cb) { cpcb = cb; }
    void setCloseBySockCallBack(const closeBySockCallBack& cb) { cscb = cb; }
private:
    void checkCallBack();
    void onConnect(int connfd);
    std::size_t getNextLoopIndex();
private:
    connectCallBack conncb;
    readCallBack readcb;
    messageCallBack msgcb;
    closeByPeerCallBack cpcb;
    closeBySockCallBack cscb;
    std::size_t index;
    std::size_t ln;
    LooperPool pool;
    int listenfd;
    Ip4Addr addr;
    bool running;
    int epfd;
    struct epoll_event poi;
    struct epoll_event evpool[SERV_EVS];
};

} // namespace pqnet

#endif // PQNET_CORE_SERVER_H
