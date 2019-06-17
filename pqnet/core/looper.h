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
    void setConnectCallBack(const connectCallBack& cb) { conncb = cb; }
    void setCloseCallBack(const closeCallBack& cb) { closecb = cb; }
    void setMessageArrivedCallBack(const messageArrivedCallBack& cb) { macb = cb; }
    void setWriteCompletedCallBack(const writeCompletedCallBack& cb) { wccb = cb; }
public:
    connectCallBack conncb;
    closeCallBack closecb;
    messageArrivedCallBack macb;
    writeCompletedCallBack wccb;
    int evfd;
    std::uint64_t msg;
    std::queue<int> waitconns;
    std::map<int, TcpConnPtr> connpool;
    int epfd;
    struct epoll_event poi;
    struct epoll_event evpool[EPOLLSIZE];
private:
    pthread_t id;
    pn_thread_func_t func;
};

} // namespace pqnet

#endif // PQNET_CORE_LOOPER_H
