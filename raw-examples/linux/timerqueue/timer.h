#ifndef TIMER_H
#define TIMER_H

#include <cstdint>
#include <functional>

using uint = unsigned int;
using TimerId = std::uint64_t;
using timerCallBack = std::function<void(void*)>;

class Timer
{
public:
    Timer(const timerCallBack& cb, void *_arg, uint _interval)
        : timercb(cb), arg(_arg), interval(_interval)
    {
        id = reinterpret_cast<TimerId>(this);
    }
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

#endif // TIMER_H
