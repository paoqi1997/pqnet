#ifndef PQNET_UTIL_TIMESTAMP_H
#define PQNET_UTIL_TIMESTAMP_H

#include <cstdint>
#include <ctime>

#include <utility>

#include <sys/time.h>

#include "types.h"

namespace pqnet
{

using uint = unsigned int;

struct TimeStamp
{
    uint Second() const { return tv.tv_sec; }
    uint Millisecond() const { return tv.tv_usec / 1000; }
    uint Microsecond() const { return tv.tv_usec; }
    uint Nanosecond() const { return tv.tv_usec * 1000; }
    std::uint64_t Int10() const { return tv.tv_sec; }
    std::uint64_t Int13() const { return tv.tv_sec * 1000 + tv.tv_usec / 1000; }
    std::uint64_t Int16() const { return tv.tv_sec * 1000000 + tv.tv_usec; }
    std::uint64_t Int19() const { return tv.tv_sec * 1000000000 + tv.tv_usec * 1000; }
    const char* toDate();
    const char* toDefault();
    const char* toString(const char *format);
    char buf[BUFSIZE];
    struct timeval tv;
    std::tm *group;
};

TimeStamp now();

std::pair<uint, uint> ms2SecAndNsec(uint ms);

} // namespace pqnet

#endif // PQNET_UTIL_TIMESTAMP_H
