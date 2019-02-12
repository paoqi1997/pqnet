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
#define TS_NOTSET(ts, msg) \
{                          \
    ts.cond.lock();        \
    fprintf(stdout, "[Notset] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg); \
    ts.cond.unlock();      \
}

#define TS_DEBUG(ts, msg) \
{                         \
    ts.cond.lock();       \
    fprintf(stdout, "[Debug] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg); \
    ts.cond.unlock();     \
}

#define TS_INFO(ts, msg) \
{                        \
    ts.cond.lock();      \
    fprintf(stdout, "[Info] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg); \
    ts.cond.unlock();    \
}

#define TS_WARNING(ts, msg) \
{                           \
    ts.cond.lock();         \
    fprintf(stdout, "[Warning] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg); \
    ts.cond.unlock();       \
}

#define TS_ERROR(ts, msg) \
{                         \
    ts.cond.lock();       \
    fprintf(stderr, "[Error] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg); \
    ts.cond.unlock();     \
}

#define TS_FATAL(ts, msg) \
{                         \
    ts.cond.lock();       \
    fprintf(stderr, "[Fatal] %s %s:%d: %s\n", pqnet::now().toDefault(), __FILE__, __LINE__, msg); \
    ts.cond.unlock();     \
}

// Async Log
#define ALOG_NOTSET(al, msg) \
{                            \
    al.pushMsg(__FILE__, __LINE__, pqnet::Logger::NOTSET, msg); \
}

#define ALOG_DEBUG(al, msg) \
{                           \
    al.pushMsg(__FILE__, __LINE__, pqnet::Logger::DEBUG, msg); \
}

#define ALOG_INFO(al, msg) \
{                          \
    al.pushMsg(__FILE__, __LINE__, pqnet::Logger::INFO, msg); \
}

#define ALOG_WARNING(al, msg) \
{                             \
    al.pushMsg(__FILE__, __LINE__, pqnet::Logger::WARNING, msg); \
}

#define ALOG_ERROR(al, msg) \
{                           \
    al.pushMsg(__FILE__, __LINE__, pqnet::Logger::ERROR, msg); \
}

#define ALOG_FATAL(al, msg) \
{                           \
    al.pushMsg(__FILE__, __LINE__, pqnet::Logger::FATAL, msg); \
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
