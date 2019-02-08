#include <cstdio>
#include <ctime>

#include "timestamp.h"

namespace pqnet
{

const char* TimeStamp::toDate()
{
    std::snprintf(buf, sizeof(buf), "%d-%02d-%02d", grp->tm_year, grp->tm_mon, grp->tm_mday);
    return buf;
}

const char* TimeStamp::toDefault()
{
    std::snprintf(buf, sizeof(buf), "%d-%02d-%02d %02d:%02d:%02d",
        grp->tm_year, grp->tm_mon, grp->tm_mday, grp->tm_hour, grp->tm_min, grp->tm_sec);
    return buf;
}

const char* TimeStamp::toString(const char *format)
{
    if (std::strftime(buf, sizeof(buf), format, grp) != 0) {
        return buf;
    } else {
        return nullptr;
    }
}

TimeStamp now()
{
    struct TimeStamp ts;
    ts.sec = std::time(nullptr);
    ts.grp = std::localtime(&ts.sec);
    ts.grp->tm_year += 1900;
    ts.grp->tm_mon += 1;
    return ts;
}

} // namespace pqnet
