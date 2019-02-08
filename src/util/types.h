#ifndef PQNET_UTIL_TYPES_H
#define PQNET_UTIL_TYPES_H

#include <cstdio>
#include <ctime>

namespace pqnet
{

#define BUFSIZE 32

using pn_file_ptr = std::FILE*;

using pn_time_sec_t   = std::time_t;
using pn_time_grp_ptr = std::tm*;

} // namespace pqnet

#endif // PQNET_UTIL_TYPES_H
