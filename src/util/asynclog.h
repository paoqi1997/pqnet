#ifndef PQNET_UTIL_ASYNC_LOG_H
#define PQNET_UTIL_ASYNC_LOG_H

#include <cstdio>

#include <queue>
#include <string>

#include "logger.h"
#include "mutex.h"

namespace pqnet
{

// Thread Safe
#define TS_TRACE(mtx, fmt, ...)                                                  \
{                                                                                \
    mtx.lock();                                                                  \
    auto logger = pqnet::Logger::getLogger();                                    \
    logger->log(__FILE__, __LINE__, pqnet::Logger::TRACE, fmt, ##__VA_ARGS__);   \
    mtx.unlock();                                                                \
}

#define TS_DEBUG(mtx, fmt, ...)                                                  \
{                                                                                \
    mtx.lock();                                                                  \
    auto logger = pqnet::Logger::getLogger();                                    \
    logger->log(__FILE__, __LINE__, pqnet::Logger::DEBUG, fmt, ##__VA_ARGS__);   \
    mtx.unlock();                                                                \
}

#define TS_INFO(mtx, fmt, ...)                                                   \
{                                                                                \
    mtx.lock();                                                                  \
    auto logger = pqnet::Logger::getLogger();                                    \
    logger->log(__FILE__, __LINE__, pqnet::Logger::INFO, fmt, ##__VA_ARGS__);    \
    mtx.unlock();                                                                \
}

#define TS_WARNING(mtx, fmt, ...)                                                \
{                                                                                \
    mtx.lock();                                                                  \
    auto logger = pqnet::Logger::getLogger();                                    \
    logger->log(__FILE__, __LINE__, pqnet::Logger::WARNING, fmt, ##__VA_ARGS__); \
    mtx.unlock();                                                                \
}

#define TS_ERROR(mtx, fmt, ...)                                                  \
{                                                                                \
    mtx.lock();                                                                  \
    auto logger = pqnet::Logger::getLogger();                                    \
    logger->log(__FILE__, __LINE__, pqnet::Logger::ERROR, fmt, ##__VA_ARGS__);   \
    mtx.unlock();                                                                \
}

#define TS_FATAL(mtx, fmt, ...)                                                  \
{                                                                                \
    mtx.lock();                                                                  \
    auto logger = pqnet::Logger::getLogger();                                    \
    logger->log(__FILE__, __LINE__, pqnet::Logger::FATAL, fmt, ##__VA_ARGS__);   \
    mtx.unlock();                                                                \
}

// Async Log
#define ALOG_TRACE(al, fmt, ...)                                                \
{                                                                               \
    al.pushMsg(__FILE__, __LINE__, pqnet::Logger::TRACE, fmt, ##__VA_ARGS__);   \
}

#define ALOG_DEBUG(al, fmt, ...)                                                \
{                                                                               \
    al.pushMsg(__FILE__, __LINE__, pqnet::Logger::DEBUG, fmt, ##__VA_ARGS__);   \
}

#define ALOG_INFO(al, fmt, ...)                                                 \
{                                                                               \
    al.pushMsg(__FILE__, __LINE__, pqnet::Logger::INFO, fmt, ##__VA_ARGS__);    \
}

#define ALOG_WARNING(al, fmt, ...)                                              \
{                                                                               \
    al.pushMsg(__FILE__, __LINE__, pqnet::Logger::WARNING, fmt, ##__VA_ARGS__); \
}

#define ALOG_ERROR(al, fmt, ...)                                                \
{                                                                               \
    al.pushMsg(__FILE__, __LINE__, pqnet::Logger::ERROR, fmt, ##__VA_ARGS__);   \
}

#define ALOG_FATAL(al, fmt, ...)                                                \
{                                                                               \
    al.pushMsg(__FILE__, __LINE__, pqnet::Logger::FATAL, fmt, ##__VA_ARGS__);   \
}

struct LogMsg
{
    const char *sourcefile;
    int line;
    Logger::LogLevel level;
    std::string msg;
};

class AsyncLog
{
public:
    AsyncLog();
    ~AsyncLog();
    LogMsg take();
    void consume(LogMsg lmsg);
    void reset(const char *date);
    bool isIdle() const { return msgqueue.empty(); }
    void pushMsg(const char *sourcefile, int line, Logger::LogLevel level, const char *fmt, ...);
public:
    Mutex mtx;
private:
    std::FILE *lf;
    std::string dir;
    std::string currdate;
    std::queue<LogMsg> msgqueue;
};

} // namespace pqnet

#endif // PQNET_UTIL_ASYNC_LOG_H
