#ifndef PQNET_CORE_TIMER_H
#define PQNET_CORE_TIMER_H

#include <cstdint>

#include "callback.h"

namespace pqnet
{

class Timer
{
public:
    Timer();
private:
    timerCallBack timercb;
    std::uint64_t id;
};

} // namespace pqnet

#endif // PQNET_CORE_TIMER_H
