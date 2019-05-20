#ifndef PQNET_UTIL_ASYNC_LOG_H
#define PQNET_UTIL_ASYNC_LOG_H

#include <cstdio>
#include <queue>
#include <string>

#include "condition.h"
#include "logger.h"
#include "mutex.h"

namespace pqnet
{

#define AL_TRACE(fmt, ...)                                                           \
{                                                                                    \
    auto al = pqnet::AsyncLog::getAsyncLog();                                        \
    pthread_t id = pthread_self();                                                   \
    al->pushMsg(pqnet::Logger::TRACE, id, __FILE__, __LINE__, fmt, ##__VA_ARGS__);   \
}

#define AL_DEBUG(fmt, ...)                                                           \
{                                                                                    \
    auto al = pqnet::AsyncLog::getAsyncLog();                                        \
    pthread_t id = pthread_self();                                                   \
    al->pushMsg(pqnet::Logger::DEBUG, id, __FILE__, __LINE__, fmt, ##__VA_ARGS__);   \
}

#define AL_INFO(fmt, ...)                                                            \
{                                                                                    \
    auto al = pqnet::AsyncLog::getAsyncLog();                                        \
    pthread_t id = pthread_self();                                                   \
    al->pushMsg(pqnet::Logger::INFO, id, __FILE__, __LINE__, fmt, ##__VA_ARGS__);    \
}

#define AL_WARNING(fmt, ...)                                                         \
{                                                                                    \
    auto al = pqnet::AsyncLog::getAsyncLog();                                        \
    pthread_t id = pthread_self();                                                   \
    al->pushMsg(pqnet::Logger::WARNING, id, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
}

#define AL_ERROR(fmt, ...)                                                           \
{                                                                                    \
    auto al = pqnet::AsyncLog::getAsyncLog();                                        \
    pthread_t id = pthread_self();                                                   \
    al->pushMsg(pqnet::Logger::ERROR, id, __FILE__, __LINE__, fmt, ##__VA_ARGS__);   \
}

#define AL_FATAL(fmt, ...)                                                           \
{                                                                                    \
    auto al = pqnet::AsyncLog::getAsyncLog();                                        \
    pthread_t id = pthread_self();                                                   \
    al->pushMsg(pqnet::Logger::FATAL, id, __FILE__, __LINE__, fmt, ##__VA_ARGS__);   \
}

struct LogMsg
{
    const char *sourcefile;
    int line;
    Logger::LogLevel level;
    pthread_t id;
    std::string msg;
};

class AsyncLog
{
public:
    static AsyncLog* getAsyncLog() {
        if (instance == nullptr) {
            mtx.lock();
            if (instance == nullptr) {
                instance = new AsyncLog();
                instance->run();
            }
            mtx.unlock();
        }
        return instance;
    }
    void run();
    void shutdown();
    static void* routine(void *arg);
    LogMsg take();
    void consume(LogMsg lmsg);
    void reset(const char *date);
    bool isIdle() const { return msgqueue.empty(); }
    void pushMsg(Logger::LogLevel level, pthread_t _id, const char *sourcefile, int line, const char *fmt, ...);
public:
    static Mutex mtx;
    Condition cond;
    bool running;
private:
    Logger::LogLevel level;
    std::string dir;
    std::string currdate;
    bool tofile;
    std::FILE *lf;
    pthread_t id;
    std::queue<LogMsg> msgqueue;
    AsyncLog();
    ~AsyncLog();
    static AsyncLog *instance;
    // To delete the instance
    class Garbo
    {
    public:
        ~Garbo() {
            auto al = AsyncLog::instance;
            if (al) {
                if (al->tofile) {
                    if (std::fclose(al->lf) != 0) {
                        ERROR(std::strerror(errno));
                    }
                }
                delete al;
                al = nullptr;
            }
        }
    };
    static Garbo garbo;
};

} // namespace pqnet

#endif // PQNET_UTIL_ASYNC_LOG_H
