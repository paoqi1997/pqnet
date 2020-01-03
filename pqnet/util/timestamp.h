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
    uint Millisecond() const { return tv.tv_usec / K1E3; }
    uint Microsecond() const { return tv.tv_usec; }
    uint Nanosecond() const { return tv.tv_usec * K1E3; }
    std::uint64_t Int10() const { return tv.tv_sec; }
    std::uint64_t Int13() const { return tv.tv_sec * K1E3 + tv.tv_usec / K1E3; }
    std::uint64_t Int16() const { return tv.tv_sec * K1E6 + tv.tv_usec; }
    std::uint64_t Int19() const { return tv.tv_sec * K1E9 + tv.tv_usec * K1E3; }
    const char* toDate();
    const char* toDefault();
    const char* toFmtStr(const char *format);
    char buf[BUFSIZE];
    struct timeval tv;
    std::tm *group;
};

TimeStamp now();

std::pair<uint, uint> ms2SecAndNsec(uint ms);
std::pair<uint, uint> us2SecAndNsec(uint us);

} // namespace pqnet

#endif // PQNET_UTIL_TIMESTAMP_H
