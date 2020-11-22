#ifndef UTIL_H
#define UTIL_H

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <iostream>

#define BACKLOG  16
#define BUFSIZE  1024
#define UNIXSOCK "/tmp/us.sock"

inline void error(const char *callname)
{
    std::printf(
        "Failed to call %s(), error: %s\n", callname, std::strerror(errno)
    );
}

#endif // UTIL_H
