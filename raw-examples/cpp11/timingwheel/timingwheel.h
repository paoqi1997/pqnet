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
public:
    std::size_t Round;
    std::size_t SlotIdx;
private:
    timerCallBack timercb;
    void *arg;
    TimerId id;
    uint interval;
};

class TimId
{
public:
    TimId() : tim(nullptr) {}
    TimId(Tim *_tim) : tim(_tim) {}
    Tim* getPtr() { return tim; }
private:
    Tim *tim;
};

class TimingWheel
{
public:
    TimingWheel(std::size_t _tick) : tick(_tick), slotIdx(0), rhTimerId(0) {}
    TimId addTimer(const timerCallBack& cb, void *arg, uint expiration, uint interval = 0);
    void delTimer(TimId tid);
    void handle();
    std::size_t getTick() const { return tick; }
private:
    static const std::size_t SlotNum = 60;
private:
    std::size_t tick;
    std::size_t slotIdx;
    TimerId rhTimerId;
    std::array<std::list<Tim>, SlotNum> slots;
};

#endif // TIMING_WHEEL_H
