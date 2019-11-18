#ifndef TIMING_WHEEL_H
#define TIMING_WHEEL_H

#include <array>
#include <cstdint>
#include <functional>
#include <list>

using uint = unsigned int;
using TimerId = std::uint64_t;
using timerCallBack = std::function<void(void*)>;

class Tim
{
public:
    Tim(const timerCallBack& cb, void *_arg, uint _interval);
    void run() { timercb(arg); }
    TimerId Id() const { return id; }
    uint Interval() const { return interval; }
    bool isPeriodic() const { return interval > 0; }
    void invalidate() { valid = false; }
    bool isValid() const { return valid; }
public:
    std::size_t Round;
    std::size_t SlotIdx;
private:
    timerCallBack timercb;
    void *arg;
    TimerId id;
    uint interval;
    bool valid;
};

class TimingWheel
{
public:
    TimingWheel(std::size_t _tick) : tick(_tick), slotIdx(0) {}
    Tim* addTimer(const timerCallBack& cb, void *arg, uint expiration, uint interval = 0);
    void delTimer(Tim *tim);
    void handle();
    std::size_t getTick() const { return tick; }
private:
    static const std::size_t SlotNum = 60;
private:
    std::size_t tick;
    std::size_t slotIdx;
    std::array<std::list<Tim>, SlotNum> slots;
};

#endif // TIMING_WHEEL_H
