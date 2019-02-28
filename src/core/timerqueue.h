#ifndef PQNET_CORE_TIMER_QUEUE_H
#define PQNET_CORE_TIMER_QUEUE_H

#include <cstdint>

#include "callback.h"
#include "timer.h"

namespace pqnet
{

class TimerQueue
{
public:
    TimerQueue();
    int getFd() const { return tmfd; }
    // Millisecond(s)
    void addTimer(const timerCallBack& cb, unsigned int expiration, unsigned int interval = 0);
    void delTimer(std::uint64_t id);
    void handle();
private:
    int tmfd;
    std::map<std::uint64_t, Timer> timerqueue;
};

} // namespace pqnet

#endif // PQNET_CORE_TIMER_QUEUE_H
