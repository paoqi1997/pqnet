#ifndef PQNET_UTIL_LOGGER_H
#define PQNET_UTIL_LOGGER_H

#include <cstdio>

#include "timestamp.h"
#include "types.h"

namespace pqnet
{

// To Terminal
#define TRACE(fmt, ...)                                                                     \
{                                                                                           \
    pqnet::Logger::fastLog(__FILE__, __LINE__, pqnet::Logger::TRACE, fmt, ##__VA_ARGS__);   \
}

#define DEBUG(fmt, ...)                                                                     \
{                                                                                           \
    pqnet::Logger::fastLog(__FILE__, __LINE__, pqnet::Logger::DEBUG, fmt, ##__VA_ARGS__);   \
}

#define INFO(fmt, ...)                                                                      \
{                                                                                           \
    pqnet::Logger::fastLog(__FILE__, __LINE__, pqnet::Logger::INFO, fmt, ##__VA_ARGS__);    \
}

#define WARNING(fmt, ...)                                                                   \
{                                                                                           \
    pqnet::Logger::fastLog(__FILE__, __LINE__, pqnet::Logger::WARNING, fmt, ##__VA_ARGS__); \
}

#define ERROR(fmt, ...)                                                                     \
{                                                                                           \
    pqnet::Logger::fastLog(__FILE__, __LINE__, pqnet::Logger::ERROR, fmt, ##__VA_ARGS__);   \
}

#define FATAL(fmt, ...)                                                                     \
{                                                                                           \
    pqnet::Logger::fastLog(__FILE__, __LINE__, pqnet::Logger::FATAL, fmt, ##__VA_ARGS__);   \
}

// To Log File
#define LOG_TRACE(fmt, ...)                                                            \
{                                                                                      \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::TRACE).log(fmt, ##__VA_ARGS__);   \
}

#define LOG_DEBUG(fmt, ...)                                                            \
{                                                                                      \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::DEBUG).log(fmt, ##__VA_ARGS__);   \
}

#define LOG_INFO(fmt, ...)                                                             \
{                                                                                      \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::INFO).log(fmt, ##__VA_ARGS__);    \
}

#define LOG_WARNING(fmt, ...)                                                          \
{                                                                                      \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::WARNING).log(fmt, ##__VA_ARGS__); \
}

#define LOG_ERROR(fmt, ...)                                                            \
{                                                                                      \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::ERROR).log(fmt, ##__VA_ARGS__);   \
}

#define LOG_FATAL(fmt, ...)                                                            \
{                                                                                      \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::FATAL).log(fmt, ##__VA_ARGS__);   \
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
    static void fastLog(const char *_sourcefile, int _line, LogLevel _level, const char *fmt, ...);
private:
    const char *sourcefile;
    int line;
    LogLevel level;
    std::FILE *lf;
};

} // namespace pqnet

#endif // PQNET_UTIL_LOGGER_H
