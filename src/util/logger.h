#ifndef PQNET_UTIL_LOGGER_H
#define PQNET_UTIL_LOGGER_H

#include <cstdio>

#include "timestamp.h"
#include "types.h"

namespace pqnet
{

// To Terminal
#define TRACE(msg)                                                                                \
{                                                                                                 \
    fprintf(stdout, "[Trace] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg); \
}

#define DEBUG(msg)                                                                                \
{                                                                                                 \
    fprintf(stdout, "[Debug] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg); \
}

#define INFO(msg)                                                                                \
{                                                                                                \
    fprintf(stdout, "[Info] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg); \
}

#define WARNING(msg)                                                                                \
{                                                                                                   \
    fprintf(stdout, "[Warning] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg); \
}

#define ERROR(msg)                                                                                \
{                                                                                                 \
    fprintf(stderr, "[Error] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg); \
}

#define FATAL(msg)                                                                                \
{                                                                                                 \
    fprintf(stderr, "[Fatal] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg); \
}

// To Log File
#define LOG_TRACE(msg)                                                \
{                                                                     \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::TRACE).log(msg); \
}

#define LOG_DEBUG(msg)                                                \
{                                                                     \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::DEBUG).log(msg); \
}

#define LOG_INFO(msg)                                                \
{                                                                    \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::INFO).log(msg); \
}

#define LOG_WARNING(msg)                                                \
{                                                                       \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::WARNING).log(msg); \
}

#define LOG_ERROR(msg)                                                \
{                                                                     \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::ERROR).log(msg); \
}

#define LOG_FATAL(msg)                                                \
{                                                                     \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::FATAL).log(msg); \
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
    void log(const char *msg) const;
private:
    const char *sourcefile;
    int line;
    LogLevel level;
    std::FILE *lf;
};

} // namespace pqnet

#endif // PQNET_UTIL_LOGGER_H
