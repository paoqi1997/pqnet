#ifndef PQNET_UTIL_TIMESTAMP_H
#define PQNET_UTIL_TIMESTAMP_H

#include <ctime>

#include "types.h"

namespace pqnet
{

struct TimeStamp
{
    const char* toDate();
    const char* toDefault();
    const char* toString(const char *format);
    char buf[BUFSIZE];
    std::time_t sec;
    std::tm *group;
};

TimeStamp now();

} // namespace pqnet

#endif // PQNET_UTIL_TIMESTAMP_H
