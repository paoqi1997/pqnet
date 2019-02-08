#ifndef PQNET_UTIL_LOGGER_H
#define PQNET_UTIL_LOGGER_H

#include <cstdio>

#include "timestamp.h"
#include "types.h"

namespace pqnet
{

#define INFO(msg) \
{                 \
    fprintf(stdout, "[Info] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg); \
}

#define ERROR(msg) \
{                  \
    fprintf(stderr, "[Error] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg); \
}

#define LOG_INFO(msg) \
{                     \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::INFO).info(msg); \
}

#define LOG_ERROR(msg) \
{                      \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::ERROR).error(msg); \
}

class Logger
{
public:
    enum LogLevel {
        NOTSET,
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL,
        NUMS_LOG_LEVEL
    };
    Logger(const char *_sourcefile, int _line, LogLevel _level);
    ~Logger();
    void info(const char *msg);
    void error(const char *msg);
private:
    const char *sourcefile;
    int line;
    LogLevel level;
    pn_file_ptr lf;
};

} // namespace pqnet

#endif // PQNET_UTIL_LOGGER_H
