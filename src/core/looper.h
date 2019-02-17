#ifndef PQNET_CORE_LOOPER_H
#define PQNET_CORE_LOOPER_H

#include <map>
#include <queue>

#include <pthread.h>
#include <sys/epoll.h>

#include "../util/types.h"
#include "callback.h"
#include "connection.h"

namespace pqnet
{

class LooperPool;

class Looper
{
public:
    Looper(LooperPool *_poolptr);
    Looper(LooperPool *_poolptr, pn_thread_func _func);
    ~Looper();
    void run();
    void shutdown();
    static void* routine(void *arg);
    pthread_t getId() const { return id; }
    LooperPool* getPool() const { return poolptr; }
    void setFunc(pn_thread_func _func) { func = _func; }
    void onConnect(const TcpConnPtr& conn) { conncb(conn); }
    void onClose(const TcpConnPtr& conn) { closecb(conn); }
    void onRead(const TcpConnPtr& conn) { readcb(conn); }
    void onMessage(const TcpConnPtr& conn) { msgcb(conn); }
    void setConnectCallBack(const connectCallBack& cb) { conncb = cb; }
    void setCloseCallBack(const closeCallBack& cb) { closecb = cb; }
    void setReadCallBack(const readCallBack& cb) { readcb = cb; }
    void setMessageCallBack(const messageCallBack& cb) { msgcb = cb; }
public:
    connectCallBack conncb;
    closeCallBack closecb;
    readCallBack readcb;
    messageCallBack msgcb;
    int evfd;
    std::queue<int> waitconns;
    std::map<int, TcpConnPtr> connpool;
    int epfd;
    struct epoll_event poi;
    struct epoll_event evpool[MAXEVENTS];
private:
    pthread_t id;
    LooperPool *poolptr;
    pn_thread_func func;

};

} // namespace pqnet

#endif // PQNET_CORE_LOOPER_H
