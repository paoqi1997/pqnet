#ifndef PQNET_UTIL_LOGGER_H
#define PQNET_UTIL_LOGGER_H

#include <cstdio>

#include "timestamp.h"
#include "types.h"

namespace pqnet
{

// To Terminal
#define NOTSET(msg) \
{                   \
    fprintf(stdout, "[Notset] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg); \
}

#define DEBUG(msg) \
{                  \
    fprintf(stdout, "[Debug] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg); \
}

#define INFO(msg) \
{                 \
    fprintf(stdout, "[Info] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg); \
}

#define WARNING(msg) \
{                    \
    fprintf(stdout, "[Warning] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg); \
}

#define ERROR(msg) \
{                  \
    fprintf(stderr, "[Error] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg); \
}

#define FATAL(msg) \
{                  \
    fprintf(stderr, "[Fatal] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg); \
}

// To Log File
#define LOG_NOTSET(msg) \
{                       \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::NOTSET).notset(msg); \
}

#define LOG_DEBUG(msg) \
{                      \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::DEBUG).debug(msg); \
}

#define LOG_INFO(msg) \
{                     \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::INFO).info(msg); \
}

#define LOG_WARNING(msg) \
{                        \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::WARNING).warning(msg); \
}

#define LOG_ERROR(msg) \
{                      \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::ERROR).error(msg); \
}

#define LOG_FATAL(msg) \
{                      \
    pqnet::Logger(__FILE__, __LINE__, pqnet::Logger::FATAL).fatal(msg); \
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
    void notset(const char *msg) const;
    void debug(const char *msg) const;
    void info(const char *msg) const;
    void warning(const char *msg) const;
    void error(const char *msg) const;
    void fatal(const char * msg) const;
private:
    const char *sourcefile;
    int line;
    LogLevel level;
    pn_file_ptr lf;
};

} // namespace pqnet

#endif // PQNET_UTIL_LOGGER_H
