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

const char* TimeStamp::toFmtStr(const char *format)
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
    uint sec = ms / 1e3;
    uint nsec = (ms - sec * 1e3) * 1e6;
    return std::make_pair(sec, nsec);
}

std::pair<uint, uint> pqnet::us2SecAndNsec(uint us)
{
    uint sec = us / 1e6;
    uint nsec = (us - sec * 1e6) * 1e3;
    return std::make_pair(sec, nsec);
}