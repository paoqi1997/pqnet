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
    TimerId getId() const { return id; }
    bool isPeriodic() const { return interval > 0; }
    uint getInterval() const { return interval; }
private:
    timerCallBack timercb;
    void *arg;
    TimerId id;
    uint interval;
};

inline bool operator < (const Timer& lhs, const Timer& rhs)
{
    return lhs.getId() < rhs.getId();
}

} // namespace pqnet

#endif // PQNET_CORE_TIMER_H
