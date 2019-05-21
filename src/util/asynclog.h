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
    al->addInfo(pqnet::Logger::TRACE, id, __FILE__, __LINE__, fmt, ##__VA_ARGS__);   \
}

#define AL_DEBUG(fmt, ...)                                                           \
{                                                                                    \
    auto al = pqnet::AsyncLog::getAsyncLog();                                        \
    pthread_t id = pthread_self();                                                   \
    al->addInfo(pqnet::Logger::DEBUG, id, __FILE__, __LINE__, fmt, ##__VA_ARGS__);   \
}

#define AL_INFO(fmt, ...)                                                            \
{                                                                                    \
    auto al = pqnet::AsyncLog::getAsyncLog();                                        \
    pthread_t id = pthread_self();                                                   \
    al->addInfo(pqnet::Logger::INFO, id, __FILE__, __LINE__, fmt, ##__VA_ARGS__);    \
}

#define AL_WARNING(fmt, ...)                                                         \
{                                                                                    \
    auto al = pqnet::AsyncLog::getAsyncLog();                                        \
    pthread_t id = pthread_self();                                                   \
    al->addInfo(pqnet::Logger::WARNING, id, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
}

#define AL_ERROR(fmt, ...)                                                           \
{                                                                                    \
    auto al = pqnet::AsyncLog::getAsyncLog();                                        \
    pthread_t id = pthread_self();                                                   \
    al->addInfo(pqnet::Logger::ERROR, id, __FILE__, __LINE__, fmt, ##__VA_ARGS__);   \
}

#define AL_FATAL(fmt, ...)                                                           \
{                                                                                    \
    auto al = pqnet::AsyncLog::getAsyncLog();                                        \
    pthread_t id = pthread_self();                                                   \
    al->addInfo(pqnet::Logger::FATAL, id, __FILE__, __LINE__, fmt, ##__VA_ARGS__);   \
}

struct LogInfo
{
    Logger::LogLevel level;
    pthread_t id;
    const char *sourcefile;
    int line;
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
            }
            mtx.unlock();
        }
        return instance;
    }
    static void* routine(void *arg);
    LogInfo take();
    void consume(LogInfo info);
    void checkLogName();
    bool isRunning() const { return running; }
    bool isIdle() const { return infoq.empty(); }
    void addInfo(Logger::LogLevel _level, pthread_t _id, const char *sourcefile, int line, const char *fmt, ...);
public:
    Condition cond;
private:
    Logger::LogLevel level;
    pthread_t id;
    std::string dir;
    std::string currdate;
    bool tofile;
    std::FILE *lf;
    std::queue<LogInfo> infoq;
    AsyncLog();
    ~AsyncLog();
    bool running;
    static Mutex mtx;
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
