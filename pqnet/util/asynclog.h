#ifndef PQNET_UTIL_ASYNC_LOG_H
#define PQNET_UTIL_ASYNC_LOG_H

#include <atomic>
#include <condition_variable>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#include "logger.h"

namespace pqnet
{

#define AL_DEBUG(fmt, ...)                                                        \
{                                                                                 \
    auto al = pqnet::AsyncLog::getAsyncLog();                                     \
    std::thread::id id = std::this_thread::get_id();                              \
    al->addLog(pqnet::Logger::DEBUG, id, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
}

#define AL_INFO(fmt, ...)                                                         \
{                                                                                 \
    auto al = pqnet::AsyncLog::getAsyncLog();                                     \
    std::thread::id id = std::this_thread::get_id();                              \
    al->addLog(pqnet::Logger::INFO, id, __FILE__, __LINE__, fmt, ##__VA_ARGS__);  \
}

#define AL_WARN(fmt, ...)                                                         \
{                                                                                 \
    auto al = pqnet::AsyncLog::getAsyncLog();                                     \
    std::thread::id id = std::this_thread::get_id();                              \
    al->addLog(pqnet::Logger::WARN, id, __FILE__, __LINE__, fmt, ##__VA_ARGS__);  \
}

#define AL_ERROR(fmt, ...)                                                        \
{                                                                                 \
    auto al = pqnet::AsyncLog::getAsyncLog();                                     \
    std::thread::id id = std::this_thread::get_id();                              \
    al->addLog(pqnet::Logger::ERROR, id, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
}

#define AL_FATAL(fmt, ...)                                                        \
{                                                                                 \
    auto al = pqnet::AsyncLog::getAsyncLog();                                     \
    std::thread::id id = std::this_thread::get_id();                              \
    al->addLog(pqnet::Logger::FATAL, id, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
}

struct Log
{
    Logger::LogLevel level;
    std::thread::id id;
    const char *sourcefile;
    int line;
    std::string msg;
};

class AsyncLog
{
public:
    static AsyncLog* getAsyncLog() {
        if (instance == nullptr) {
            std::lock_guard<std::mutex> lk(mtx);
            if (instance == nullptr) {
                instance = new AsyncLog();
            }
        }
        return instance;
    }
    std::condition_variable& Cond() { return cond; }
    Log take();
    void consume(Log log);
    void checkLogName();
    void setLogLevel(Logger::LogLevel _level) {
        level = _level;
    }
    void setOutput(Logger::Output output);
    bool isRunning() const { return running; }
    bool isIdle() const { return logqueue.empty(); }
    void addLog(
        Logger::LogLevel _level, std::thread::id _id, const char *sourcefile, int line, const char *fmt, ...
    );
private:
    Logger::LogLevel level;
    std::string dir;
    std::string currdate;
    bool tofile;
    std::FILE *lf;
    std::atomic<bool> running;
    std::condition_variable cond;
    std::queue<Log> logqueue;
    std::unique_ptr<std::thread> thd;
private:
    AsyncLog();
    ~AsyncLog();
    static std::mutex mtx;
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
