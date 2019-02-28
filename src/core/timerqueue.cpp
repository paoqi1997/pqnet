#include <cerrno>
#include <cstring>

#include <sys/timerfd.h>

#include "../util/logger.h"
#include "timerqueue.h"

using namespace pqnet;

TimerQueue::TimerQueue()
{
    tmfd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK);
    if (tmfd == -1) {
        ERROR(std::strerror(errno));
    }
}
