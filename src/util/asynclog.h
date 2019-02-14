#ifndef PQNET_UTIL_ASYNC_LOG_H
#define PQNET_UTIL_ASYNC_LOG_H

#include <cstdio>

#include <queue>
#include <string>

#include <pthread.h>

#include "condition.h"
#include "logger.h"
#include "timestamp.h"

namespace pqnet
{

// Thread Safe
#define TS_TRACE(ts, fmt, ...)                                                              \
{                                                                                           \
    ts.cond.lock();                                                                         \
    pqnet::Logger::fastLog(__FILE__, __LINE__, pqnet::Logger::TRACE, fmt, ##__VA_ARGS__);   \
    ts.cond.unlock();                                                                       \
}

#define TS_DEBUG(ts, fmt, ...)                                                              \
{                                                                                           \
    ts.cond.lock();                                                                         \
    pqnet::Logger::fastLog(__FILE__, __LINE__, pqnet::Logger::DEBUG, fmt, ##__VA_ARGS__);   \
    ts.cond.unlock();                                                                       \
}

#define TS_INFO(ts, fmt, ...)                                                               \
{                                                                                           \
    ts.cond.lock();                                                                         \
    pqnet::Logger::fastLog(__FILE__, __LINE__, pqnet::Logger::INFO, fmt, ##__VA_ARGS__);    \
    ts.cond.unlock();                                                                       \
}

#define TS_WARNING(ts, fmt, ...)                                                            \
{                                                                                           \
    ts.cond.lock();                                                                         \
    pqnet::Logger::fastLog(__FILE__, __LINE__, pqnet::Logger::WARNING, fmt, ##__VA_ARGS__); \
    ts.cond.unlock();                                                                       \
}

#define TS_ERROR(ts, fmt, ...)                                                              \
{                                                                                           \
    ts.cond.lock();                                                                         \
    pqnet::Logger::fastLog(__FILE__, __LINE__, pqnet::Logger::ERROR, fmt, ##__VA_ARGS__);   \
    ts.cond.unlock();                                                                       \
}

#define TS_FATAL(ts, fmt, ...)                                                              \
{                                                                                           \
    ts.cond.lock();                                                                         \
    pqnet::Logger::fastLog(__FILE__, __LINE__, pqnet::Logger::FATAL, fmt, ##__VA_ARGS__);   \
    ts.cond.unlock();                                                                       \
}

// Async Log
#define ALOG_TRACE(al, msg)                                      \
{                                                                \
    al.pushMsg(__FILE__, __LINE__, pqnet::Logger::TRACE, msg);   \
}

#define ALOG_DEBUG(al, msg)                                      \
{                                                                \
    al.pushMsg(__FILE__, __LINE__, pqnet::Logger::DEBUG, msg);   \
}

#define ALOG_INFO(al, msg)                                       \
{                                                                \
    al.pushMsg(__FILE__, __LINE__, pqnet::Logger::INFO, msg);    \
}

#define ALOG_WARNING(al, msg)                                    \
{                                                                \
    al.pushMsg(__FILE__, __LINE__, pqnet::Logger::WARNING, msg); \
}

#define ALOG_ERROR(al, msg)                                      \
{                                                                \
    al.pushMsg(__FILE__, __LINE__, pqnet::Logger::ERROR, msg);   \
}

#define ALOG_FATAL(al, msg)                                      \
{                                                                \
    al.pushMsg(__FILE__, __LINE__, pqnet::Logger::FATAL, msg);   \
}

struct LogMsg
{
    const char *sourcefile;
    int line;
    Logger::LogLevel level;
    const char *msg;
};

class ThreadPool;

class AsyncLog
{
public:
    AsyncLog(ThreadPool *_poolptr);
    ~AsyncLog();
    void run();
    static void* routine(void *arg);
    LogMsg take();
    void consume(LogMsg lmsg);
    void reset(const char *date);
    bool isIdle() const { return msgqueue.empty(); }
    pthread_t getId() const { return id; }
    ThreadPool* getPool() const { return poolptr; }
    void pushMsg(const char *sourcefile, int line, Logger::LogLevel level, const char *msg);
public:
    Condition cond;
private:
    pthread_t id;
    ThreadPool *poolptr;
    std::FILE *lf;
    std::string dir;
    std::string currdate;
    std::queue<LogMsg> msgqueue;
};

} // namespace pqnet

#endif // PQNET_UTIL_ASYNC_LOG_H
