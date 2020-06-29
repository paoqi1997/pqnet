#ifndef TIMER_QUEUE_H
#define TIMER_QUEUE_H

#include <cstdint>
#include <set>
#include <utility>

#include "timer.h"

#define K1E3 (1000)
#define K1E6 (1000 * 1000)
#define K1E9 (1000 * 1000 * 1000)

std::uint64_t now();

std::pair<uint, uint> ms2SecAndNsec(uint ms);
std::pair<uint, uint> us2SecAndNsec(uint us);

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
    bool isRmHead() const { return rhTimerId != 0; }
private:
    int tmfd;
    TimerId rhTimerId;
    std::set<std::pair<std::uint64_t, Timer>> tmqueue;
};

#endif // TIMER_QUEUE_H
