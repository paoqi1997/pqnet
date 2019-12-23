#ifndef PQNET_CORE_EVENTLOOP_H
#define PQNET_CORE_EVENTLOOP_H

#include <cstdint>
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
    TimerId addTimer(const timerCallBack& cb, void *arg, uint _expiration, uint _interval = 0) {
        return tmqueue->addTimer(cb, arg, _expiration, _interval);
    }
    void delTimer(TimerId id) {
        tmqueue->delTimer(id);
    }
    void pushFn(const Functor& fn) { fnQueue.push(fn); }
    Functor popFn() {
        auto fn = fnQueue.front();
        fnQueue.pop();
        return fn;
    }
private:
    void handleRead();
    struct epoll_event* begin() { return evpool.data(); }
    const struct epoll_event* begin() const { return evpool.data(); }
private:
    int epfd;
    int evfd;
    bool loopFlag;
    std::uint64_t msg;
    std::queue<Functor> fnQueue;
    std::unique_ptr<Trigger> evTrigger;
    std::unique_ptr<Trigger> tmTrigger;
    std::unique_ptr<TimerQueue> tmqueue;
    std::vector<struct epoll_event> evpool;
};

} // namespace pqnet

#endif // PQNET_CORE_EVENTLOOP_H
