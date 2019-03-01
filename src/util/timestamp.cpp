#include <cstdio>

#include "timestamp.h"

using namespace pqnet;

const char* TimeStamp::toDate()
{
    std::snprintf(buf, sizeof(buf), "%d-%02d-%02d", group->tm_year, group->tm_mon, group->tm_mday);
    return buf;
}

const char* TimeStamp::toDefault()
{
    std::snprintf(buf, sizeof(buf), "%d-%02d-%02d %02d:%02d:%02d:%06ld",
        group->tm_year, group->tm_mon, group->tm_mday, group->tm_hour, group->tm_min, group->tm_sec, tv.tv_usec);
    return buf;
}

const char* TimeStamp::toString(const char *format)
{
    if (std::strftime(buf, sizeof(buf), format, group) != 0) {
        return buf;
    } else {
        return nullptr;
    }
}

TimeStamp pqnet::now()
{
    struct TimeStamp ts;
    gettimeofday(&ts.tv, nullptr);
    ts.group = std::localtime(&ts.tv.tv_sec);
    ts.group->tm_year += 1900;
    ts.group->tm_mon += 1;
    return ts;
}

std::pair<uint, uint> pqnet::ms2SecAndNsec(uint ms)
{
    uint sec = ms / 1000;
    uint nsec = (ms - sec * 1000) * 1000 * 1000;
    return std::make_pair(sec, nsec);
}
