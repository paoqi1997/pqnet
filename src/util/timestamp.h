#ifndef PQNET_UTIL_TIMESTAMP_H
#define PQNET_UTIL_TIMESTAMP_H

#include <cstdint>
#include <ctime>

#include <utility>

#include <sys/time.h>

#include "types.h"

namespace pqnet
{

struct TimeStamp
{
    unsigned int Second() const { return tv.tv_sec; }
    unsigned int Millisecond() const { return tv.tv_usec / 1000; }
    unsigned int Microsecond() const { return tv.tv_usec; }
    unsigned int Nanosecond() const { return tv.tv_usec * 1000; }
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

std::pair<unsigned int, unsigned int> ms2SecAndNsec(unsigned int ms);

} // namespace pqnet

#endif // PQNET_UTIL_TIMESTAMP_H
