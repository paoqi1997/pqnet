#include <cstdio>
#include <ctime>

#include "timestamp.h"

namespace pqnet
{

const char* TimeStamp::toDate()
{
    std::snprintf(buf, sizeof(buf), "%d-%02d-%02d", group->tm_year, group->tm_mon, group->tm_mday);
    return buf;
}

const char* TimeStamp::toDefault()
{
    std::snprintf(buf, sizeof(buf), "%d-%02d-%02d %02d:%02d:%02d",
        group->tm_year, group->tm_mon, group->tm_mday, group->tm_hour, group->tm_min, group->tm_sec);
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

TimeStamp now()
{
    struct TimeStamp ts;
    ts.sec = std::time(nullptr);
    ts.group = std::localtime(&ts.sec);
    ts.group->tm_year += 1900;
    ts.group->tm_mon += 1;
    return ts;
}

} // namespace pqnet
