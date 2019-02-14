#ifndef PQNET_UTIL_LOGGER_H
#define PQNET_UTIL_LOGGER_H

#include <cstdio>

#include "timestamp.h"
#include "types.h"

namespace pqnet
{

// To Terminal
#define TRACE(msg)                                                                                  \
{                                                                                                   \
    fprintf(stdout, "[Trace] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg);   \
}

#define DEBUG(msg)                                                                                  \
{                                                                                                   \
    fprintf(stdout, "[Debug] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg);   \
}

#define INFO(msg)                                                                                   \
{                                                                                                   \
    fprintf(stdout, "[Info] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg);    \
}

#define WARNING(msg)                                                                                \
{                                                                                                   \
    fprintf(stdout, "[Warning] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg); \
}

#define ERROR(msg)                                                                                  \
{                                                                                                   \
    fprintf(stderr, "[Error] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg);   \
}

#define FATAL(msg)                                                                                  \
{                                                                                                   \
    fprintf(stderr, "[Fatal] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg);   \
}

// To Log File
#define LOG_TRACE(fmt, ...)                                                          \
{                                                                                    \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::TRACE).log(fmt, __VA_ARGS__);   \
}

#define LOG_DEBUG(fmt, ...)                                                          \
{                                                                                    \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::DEBUG).log(fmt, __VA_ARGS__);   \
}

#define LOG_INFO(fmt, ...)                                                           \
{                                                                                    \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::INFO).log(fmt, __VA_ARGS__);    \
}

#define LOG_WARNING(fmt, ...)                                                        \
{                                                                                    \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::WARNING).log(fmt, __VA_ARGS__); \
}

#define LOG_ERROR(fmt, ...)                                                          \
{                                                                                    \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::ERROR).log(fmt, __VA_ARGS__);   \
}

#define LOG_FATAL(fmt, ...)                                                          \
{                                                                                    \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::FATAL).log(fmt, __VA_ARGS__);   \
}

class Logger
{
public:
    enum LogLevel {
        TRACE,
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };
    Logger(const char *_sourcefile, int _line, LogLevel _level);
    ~Logger();
    void log(const char *fmt, ...) const;
private:
    const char *sourcefile;
    int line;
    LogLevel level;
    std::FILE *lf;
};

} // namespace pqnet

#endif // PQNET_UTIL_LOGGER_H
