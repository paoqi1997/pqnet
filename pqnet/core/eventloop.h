#ifndef PQNET_CORE_EVENTLOOP_H
#define PQNET_CORE_EVENTLOOP_H

#include <functional>
#include <memory>
#include <queue>
#include <vector>

#include <sys/epoll.h>

#include "timerqueue.h"
#include "trigger.h"

namespace pqnet
{

using Functor = std::function<void()>;

class EventLoop
{
public:
    EventLoop(int eventPoolSize = 128);
    ~EventLoop();
    int getFd() const { return epfd; }
    int getEvfd() const { return evfd; }
    void loop();
    void quit() { loopFlag = false; }
    TimerId runAt(const timerCallBack& cb, void *arg, const TimeStamp& ts);
    TimerId runAfter(const timerCallBack& cb, void *arg, uint expiration);
    TimerId runEvery(const timerCallBack& cb, void *arg, uint expiration, uint interval);
    void cancel(TimerId id);
    void pushFunctor(const Functor& fn);
private:
    void wake();
    void handleRead();
    struct epoll_event* begin() { return evpool.data(); }
    const struct epoll_event* begin() const { return evpool.data(); }
private:
    int epfd;
    int evfd;
    bool loopFlag;
    std::queue<Functor> fnqueue;
    std::unique_ptr<Trigger> evTrigger;
    std::unique_ptr<Trigger> tmTrigger;
    std::unique_ptr<TimerQueue> tmqueue;
    std::vector<struct epoll_event> evpool;
};

} // namespace pqnet

#endif // PQNET_CORE_EVENTLOOP_H
