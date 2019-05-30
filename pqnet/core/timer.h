#ifndef PQNET_CORE_TIMER_H
#define PQNET_CORE_TIMER_H

#include <cstdint>

#include <functional>

#include "../util/timestamp.h"

namespace pqnet
{

using TimerId = std::uint64_t;

using timerCallBack = std::function<void(void*)>;

class Timer
{
public:
    Timer(const timerCallBack& cb, void *_arg, uint _interval);
    void run() { timercb(arg); }
    TimerId Id() const { return id; }
    uint Interval() const { return interval; }
    bool isPeriodic() const { return interval > 0; }
private:
    timerCallBack timercb;
    void *arg;
    TimerId id;
    uint interval;
};

inline bool operator < (const Timer& lhs, const Timer& rhs)
{
    return lhs.Id() < rhs.Id();
}

} // namespace pqnet

#endif // PQNET_CORE_TIMER_H
