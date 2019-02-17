#ifndef PQNET_CORE_SERVER_H
#define PQNET_CORE_SERVER_H

#include <cstdint>

#include <functional>

#include <sys/epoll.h>

#include "../util/types.h"
#include "callback.h"
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
    void shutdown();
    void setConnectCallBack(const connectCallBack& cb) { conncb = cb; }
    void setCloseCallBack(const closeCallBack& cb) { closecb = cb; }
    void setReadCallBack(const readCallBack& cb) { readcb = cb; }
    void setMessageCallBack(const messageCallBack& cb) { msgcb = cb; }
private:
    void checkCallBack();
    void onConnect(int connfd);
    std::size_t getNextLoopIndex();
private:
    connectCallBack conncb;
    closeCallBack closecb;
    readCallBack readcb;
    messageCallBack msgcb;
    int listenfd;
    std::size_t index;
    std::size_t ln;
    LooperPool pool;
    int epfd;
    struct epoll_event poi;
    struct epoll_event evpool[SERV_EVS];
};

} // namespace pqnet

#endif // PQNET_CORE_SERVER_H
