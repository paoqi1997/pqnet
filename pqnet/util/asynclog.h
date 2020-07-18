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

#define AL_DEBUG(fmt, ...)                                                         \
{                                                                                  \
    auto al = pqnet::AsyncLog::getInstance();                                      \
    std::thread::id tid = std::this_thread::get_id();                              \
    al->addLog(pqnet::Logger::DEBUG, tid, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
}

#define AL_INFO(fmt, ...)                                                          \
{                                                                                  \
    auto al = pqnet::AsyncLog::getInstance();                                      \
    std::thread::id tid = std::this_thread::get_id();                              \
    al->addLog(pqnet::Logger::INFO, tid, __FILE__, __LINE__, fmt, ##__VA_ARGS__);  \
}

#define AL_WARN(fmt, ...)                                                          \
{                                                                                  \
    auto al = pqnet::AsyncLog::getInstance();                                      \
    std::thread::id tid = std::this_thread::get_id();                              \
    al->addLog(pqnet::Logger::WARN, tid, __FILE__, __LINE__, fmt, ##__VA_ARGS__);  \
}

#define AL_ERROR(fmt, ...)                                                         \
{                                                                                  \
    auto al = pqnet::AsyncLog::getInstance();                                      \
    std::thread::id tid = std::this_thread::get_id();                              \
    al->addLog(pqnet::Logger::ERROR, tid, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
}

#define AL_FATAL(fmt, ...)                                                         \
{                                                                                  \
    auto al = pqnet::AsyncLog::getInstance();                                      \
    std::thread::id tid = std::this_thread::get_id();                              \
    al->addLog(pqnet::Logger::FATAL, tid, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
}

struct Log
{
    Log() {}
    Log(
        Logger::LogLevel _level, std::thread::id tid,
        const char *_sourcefile, int _line, const char *_msg
    )
        : level(_level), id(tid),
          sourcefile(_sourcefile), line(_line), msg(_msg) {}
    Logger::LogLevel level;
    std::thread::id id;
    const char *sourcefile;
    int line;
    std::string msg;
};

class AsyncLog
{
public:
    static AsyncLog* getInstance() {
        if (instance == nullptr) {
            std::lock_guard<std::mutex> lk(mtx);
            if (instance == nullptr) {
                instance = new AsyncLog();
            }
        }
        return instance;
    }
    std::condition_variable& Cond() { return cond; }
    void checkLogName();
    void setLogLevel(Logger::LogLevel _level) { level = _level; }
    void setTarget(Logger::Target _target);
    bool isRunning() const { return running; }
    bool isIdle() const { return logqueue.empty(); }
    void addLog(
        Logger::LogLevel _level, std::thread::id tid, const char *sourcefile, int line, const char *fmt, ...
    );
    void consume(Log log);
private:
    Log take() {
        auto log = logqueue.front();
        logqueue.pop();
        return log;
    }
private:
    Logger::LogLevel level;
    Logger::Target target;
    std::string dir;
    std::string currdate;
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
                if (al->target == Logger::FILE) {
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
