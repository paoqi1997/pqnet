#include "timer.h"

using namespace pqnet;

Timer::Timer(const timerCallBack& cb, void *_arg, uint _interval)
    : timercb(cb), arg(_arg), interval(_interval)
{
    id = reinterpret_cast<std::uint64_t>(this);
}
