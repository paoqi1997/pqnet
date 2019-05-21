#include <cerrno>
#include <cstdarg>
#include <cstring>
#include <vector>

#include <sys/stat.h>
#include <unistd.h>

#include "asynclog.h"
#include "thread.h"
#include "timestamp.h"

using namespace pqnet;

Mutex AsyncLog::mtx;
AsyncLog *AsyncLog::instance = nullptr;
AsyncLog::Garbo AsyncLog::garbo;

AsyncLog::AsyncLog()
    : running(false), level(Logger::INFO),
      dir("./log/"), currdate(now().toDate()), tofile(true)
{
    if (access(dir.c_str(), F_OK) != 0) {
        if (mkdir(dir.c_str(), 0777) != 0) {
            ERROR(std::strerror(errno));
        }
    }
    std::string lfname = dir;
    lfname += currdate + ".log";
    lf = std::fopen(lfname.c_str(), "a");
}

AsyncLog::~AsyncLog()
{
    if (running) {
        this->shutdown();
    }
}

void AsyncLog::run()
{
    running = true;
    if (pthread_create(&id, nullptr, routine, this) != 0) {
        ERROR(std::strerror(errno));
    }
}

void AsyncLog::shutdown()
{
    running = false;
    cond.notify();
    if (pthread_join(id, nullptr) != 0) {
        ERROR(std::strerror(errno));
    }
}

void* AsyncLog::routine(void *arg)
{
    auto self = static_cast<AsyncLog*>(arg);
    for (;;) {
        self->cond.lock();
        while (self->running && self->isEmpty()) {
            self->cond.wait();
        }
        if (!self->running) {
            self->cond.unlock();
            break;
        }
        auto info = self->take();
        self->cond.unlock();
        self->consume(info);
    }
    return nullptr;
}

LogInfo AsyncLog::take()
{
    auto info = infoq.front();
    infoq.pop();
    return info;
}

void AsyncLog::consume(LogInfo info)
{
    const char *date = now().toDate();
    if (std::strcmp(currdate.c_str(), date) != 0) {
        this->reset(date);
    }
    if (info.level >= level) {
        const char *time = now().toDefault();
        pthread_t id = info.id;
        const char *sourcefile = info.sourcefile;
        int line = info.line;
        const char *msg = info.msg.c_str();
        switch (info.level) {
        case Logger::TRACE:
            std::fprintf(lf, "[Trace] %s ThreadID:%ld %s:%d: %s\n", time, id, sourcefile, line, msg);
            break;
        case Logger::DEBUG:
            std::fprintf(lf, "[Debug] %s ThreadID:%ld %s:%d: %s\n", time, id, sourcefile, line, msg);
            break;
        case Logger::INFO:
            std::fprintf(lf, "[Info] %s ThreadID:%ld %s:%d: %s\n", time, id, sourcefile, line, msg);
            break;
        case Logger::WARNING:
            std::fprintf(lf, "[Warning] %s ThreadID:%ld %s:%d: %s\n", time, id, sourcefile, line, msg);
            break;
        case Logger::ERROR:
            std::fprintf(lf, "[Error] %s ThreadID:%ld %s:%d: %s\n", time, id, sourcefile, line, msg);
            break;
        case Logger::FATAL:
            std::fprintf(lf, "[Fatal] %s ThreadID:%ld %s:%d: %s\n", time, id, sourcefile, line, msg);
            break;
        }
    }
}

void AsyncLog::reset(const char *date)
{
    if (std::fclose(lf) != 0) {
        ERROR(std::strerror(errno));
    }
    currdate = date;
    std::string lfname = dir;
    lfname += currdate + ".log";
    lf = std::fopen(lfname.c_str(), "a");
}

void AsyncLog::pushMsg(Logger::LogLevel _level, pthread_t _id, const char *sourcefile, int line, const char *fmt, ...)
{
    std::va_list args1, args2;
    va_start(args1, fmt);
    va_copy(args2, args1);
    int size = std::vsnprintf(nullptr, 0, fmt, args1);
    va_end(args1);
    std::vector<char> buf(size + 1);
    std::vsprintf(buf.data(), fmt, args2);
    va_end(args2);
    LogInfo info{ _level, _id, sourcefile, line, std::string(buf.data()) };
    cond.lock();
    infoq.push(info);
    cond.unlock();
    cond.notify();
}
