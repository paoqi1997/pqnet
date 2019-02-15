#ifndef PQNET_CORE_LOOPER_H
#define PQNET_CORE_LOOPER_H

#include <map>
#include <queue>

#include <sys/epoll.h>
#include <sys/eventfd.h>

#include "../util/thread.h"
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
    ~Looper();
    void run();
    static void* routine(void *arg);
public:
    int evfd;
    std::queue<int> waitconns;
private:
    connectCallBack conncb;
    closeCallBack closecb;
    readCallBack readcb;
    writeCallBack writecb;
    Thread t;
    LooperPool *poolptr;
    std::map<int, TcpConnPtr> connpool;
    int epfd;
    struct epoll_event poi;
    struct epoll_event evpool[MAXEVENTS];
};

} // namespace pqnet

#endif // PQNET_CORE_LOOPER_H
