#ifndef PQNET_CORE_EVENTLOOP_H
#define PQNET_CORE_EVENTLOOP_H

#include <cstdint>

#include <functional>
#include <memory>
#include <vector>

#include <sys/epoll.h>

#include "trigger.h"

namespace pqnet
{

using Functor = std::function<void()>;

class EventLoop
{
public:
    EventLoop(int eventPoolSize = 128);
    ~EventLoop();
    int getFd() const { return m_epfd; }
    int getEvfd() const { return m_evfd; }
    void loop();
    void exec(Functor fn) { fn(); }
    void shutdown() { loopFlag = false; }
private:
    void handleRead();
    struct epoll_event* begin() { return evpool.data(); }
    const struct epoll_event* begin() const { return evpool.data(); }
private:
    int m_epfd;
    int m_evfd;
    bool loopFlag;
    std::uint64_t msg;
    std::unique_ptr<Trigger> evTrigger;
    std::vector<struct epoll_event> evpool;
};

} // namespace pqnet

#endif // PQNET_CORE_EVENTLOOP_H
