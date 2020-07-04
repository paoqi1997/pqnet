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
    TimerQueue() : rhTimerId(0) {}
    // Millisecond Level
    TimerId addTimer(const timerCallBack& cb, void *arg, uint _expiration, uint _interval = 0);
    void delTimer(TimerId id);
    void handle();
private:
    bool isRmHead() const { return rhTimerId != 0; }
private:
    TimerId rhTimerId;
    std::set<std::pair<std::uint64_t, Timer>> tmqueue;
};

} // namespace pqnet

#endif // PQNET_CORE_TIMER_QUEUE_H
