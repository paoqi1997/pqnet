#ifndef PQNET_CORE_LOOPER_H
#define PQNET_CORE_LOOPER_H

#include <cstdint>

#include <map>
#include <queue>

#include <pthread.h>
#include <sys/epoll.h>

#include "../util/types.h"
#include "callback.h"
#include "connection.h"

namespace pqnet
{

class Looper
{
public:
    Looper();
    Looper(pn_thread_func_t _func);
    ~Looper();
    void run();
    static void* routine(void *arg);
    pthread_t getId() const { return id; }
    void setFunc(pn_thread_func_t _func) { func = _func; }
    void onConnect(const TcpConnPtr& conn) { conncb(conn); }
    void onRead(const TcpConnPtr& conn) { readcb(conn); }
    void onMessage(const TcpConnPtr& conn) { msgcb(conn); }
    void onCloseByPeer(const TcpConnPtr& conn) { cpcb(conn); }
    void onCloseBySock(const TcpConnPtr& conn) { cscb(conn); }
    void setConnectCallBack(const connectCallBack& cb) { conncb = cb; }
    void setReadCallBack(const readCallBack& cb) { readcb = cb; }
    void setMessageCallBack(const messageCallBack& cb) { msgcb = cb; }
    void setCloseByPeerCallBack(const closeByPeerCallBack& cb) { cpcb = cb; }
    void setCloseBySockCallBack(const closeBySockCallBack& cb) { cscb = cb; }
public:
    connectCallBack conncb;
    readCallBack readcb;
    messageCallBack msgcb;
    closeByPeerCallBack cpcb;
    closeBySockCallBack cscb;
    int evfd;
    std::uint64_t msg;
    std::queue<int> waitconns;
    std::map<int, TcpConnPtr> connpool;
    int epfd;
    struct epoll_event poi;
    struct epoll_event evpool[SERV_EVS];
private:
    pthread_t id;
    pn_thread_func_t func;
};

} // namespace pqnet

#endif // PQNET_CORE_LOOPER_H
