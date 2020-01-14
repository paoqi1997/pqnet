#include <cstdio>

#include "timestamp.h"

using namespace pqnet;

const char* TimeStamp::toDate()
{
    std::snprintf(buf, sizeof(buf), "%d-%02d-%02d", group.tm_year + 1900, group.tm_mon + 1, group.tm_mday);
    return buf;
}

const char* TimeStamp::toClock()
{
    std::snprintf(buf, sizeof(buf), "%02d:%02d:%02d", group.tm_hour, group.tm_min, group.tm_sec);
    return buf;
}

const char* TimeStamp::toDefault()
{
    std::snprintf(buf, sizeof(buf), "%d-%02d-%02d %02d:%02d:%02d:%06ld",
        group.tm_year + 1900, group.tm_mon + 1, group.tm_mday, group.tm_hour, group.tm_min, group.tm_sec, tv.tv_usec);
    return buf;
}

const char* TimeStamp::toFmtStr(const char *format)
{
    if (std::strftime(buf, sizeof(buf), format, &group) != 0) {
        return buf;
    } else {
        return nullptr;
    }
}

TimeStamp pqnet::now()
{
    struct TimeStamp ts;
    gettimeofday(&ts.tv, nullptr);
    ts.group = *std::localtime(&ts.tv.tv_sec);
    return ts;
}

TimeStamp pqnet::oneday(const tm_t& box)
{
    struct TimeStamp ts;
    ts.group.tm_year = std::get<0>(box) - 1900;
    ts.group.tm_mon  = std::get<1>(box) - 1;
    ts.group.tm_mday = std::get<2>(box);
    ts.group.tm_hour = std::get<3>(box);
    ts.group.tm_min  = std::get<4>(box);
    ts.group.tm_sec  = std::get<5>(box);
    ts.tv.tv_sec = std::mktime(&ts.group);
    ts.tv.tv_usec = 0;
    return ts;
}

std::pair<uint, uint> pqnet::ms2SecAndNsec(uint ms)
{
    uint sec = ms / K1E3;
    uint nsec = (ms - sec * K1E3) * K1E6;
    return std::make_pair(sec, nsec);
}

std::pair<uint, uint> pqnet::us2SecAndNsec(uint us)
{
    uint sec = us / K1E6;
    uint nsec = (us - sec * K1E6) * K1E3;
    return std::make_pair(sec, nsec);
}
