#ifndef PQNET_CORE_TIMER_QUEUE_H
#define PQNET_CORE_TIMER_QUEUE_H

#include <cstdint>
#include <set>
#include <utility>

#include "timer.h"

namespace pqnet
{

class TimerQueue
{
public:
    TimerQueue();
    ~TimerQueue();
    int getFd() const { return tmfd; }
    // Millisecond Level
    TimerId addTimer(const timerCallBack& cb, void *arg, uint _expiration, uint _interval = 0);
    void delTimer(TimerId id);
    void handle();
private:
    void refresh();
    bool isRmHead() const { return rmHead; }
private:
    int tmfd;
    bool rmHead;
    std::set<std::pair<std::uint64_t, Timer>> tmqueue;
};

} // namespace pqnet

#endif // PQNET_CORE_TIMER_QUEUE_H
