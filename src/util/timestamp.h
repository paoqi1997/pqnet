#ifndef PQNET_UTIL_TIMESTAMP_H
#define PQNET_UTIL_TIMESTAMP_H

#include <ctime>

#include <sys/time.h>

#include "types.h"

namespace pqnet
{

struct TimeStamp
{
    const char* toDate();
    const char* toDefault();
    const char* toString(const char *format);
    char buf[BUFSIZE];
    struct timeval tv;
    std::tm *group;
};

TimeStamp now();

} // namespace pqnet

#endif // PQNET_UTIL_TIMESTAMP_H
