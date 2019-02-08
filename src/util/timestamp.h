#ifndef PQNET_UTIL_TIMESTAMP_H
#define PQNET_UTIL_TIMESTAMP_H

#include "types.h"

namespace pqnet
{

struct TimeStamp
{
    const char* toDate();
    const char* toDefault();
    const char* toString(const char *format);
    char buf[BUFSIZE];
    pn_time_sec_t sec;
    pn_time_grp_ptr grp;
};

TimeStamp now();

} // namespace pqnet

#endif // PQNET_UTIL_TIMESTAMP_H
