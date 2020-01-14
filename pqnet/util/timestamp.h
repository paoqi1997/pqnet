#ifndef PQNET_UTIL_TIMESTAMP_H
#define PQNET_UTIL_TIMESTAMP_H

#include <cstdint>
#include <ctime>
#include <tuple>
#include <utility>

#include <sys/time.h>

#include "types.h"

namespace pqnet
{

using uint = unsigned int;
using tm_t = std::tuple<int, int, int, int, int, int>;

struct TimeStamp
{
    /**
     * 返回TimeStamp对应的那一分钟的第几秒
     */
    uint Second() const { return group.tm_sec; }
    /**
     * 返回TimeStamp对应的那一秒内的毫秒偏移量
     */
    uint Millisecond() const { return tv.tv_usec / K1E3; }
    /**
     * 返回TimeStamp对应的那一秒内的微秒偏移量
     */
    uint Microsecond() const { return tv.tv_usec; }
    /**
     * 返回TimeStamp对应的那一秒内的纳秒偏移量
     */
    uint Nanosecond() const { return tv.tv_usec * K1E3; }
    std::uint64_t Int10() const { return tv.tv_sec; }
    std::uint64_t Int13() const { return tv.tv_sec * K1E3 + tv.tv_usec / K1E3; }
    std::uint64_t Int16() const { return tv.tv_sec * K1E6 + tv.tv_usec; }
    std::uint64_t Int19() const { return tv.tv_sec * K1E9 + tv.tv_usec * K1E3; }
    const char* toDate();
    const char* toClock();
    const char* toDefault();
    const char* toFmtStr(const char *format);
    struct timeval tv;
    std::tm group;
    char buf[BUFSIZE];
};

TimeStamp now();
TimeStamp oneday(const tm_t& box);

std::pair<uint, uint> ms2SecAndNsec(uint ms);
std::pair<uint, uint> us2SecAndNsec(uint us);

} // namespace pqnet

#endif // PQNET_UTIL_TIMESTAMP_H
