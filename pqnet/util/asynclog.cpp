#include <cerrno>
#include <cstdarg>
#include <cstring>
#include <string>
#include <vector>

#include <pthread.h>
#include <sys/stat.h>
#include <unistd.h>

#include "asynclog.h"
#include "timestamp.h"

using namespace pqnet;

Mutex AsyncLog::mtx;
AsyncLog *AsyncLog::instance = nullptr;
AsyncLog::Garbo AsyncLog::garbo;

AsyncLog::AsyncLog()
    : level(Logger::INFO), dir("./log/"),
      currdate(now().toDate()), tofile(false), running(true)
{
    lf = stdout;
    if (access(dir.c_str(), F_OK) != 0) {
        if (mkdir(dir.c_str(), 0777) != 0) {
            ERROR(std::strerror(errno));
        }
    }
    if (pthread_create(&id, nullptr, routine, this) != 0) {
        ERROR(std::strerror(errno));
    }
}

AsyncLog::~AsyncLog()
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
        while (self->isRunning() && self->isIdle()) {
            self->cond.wait();
        }
        if (!self->isRunning()) {
            self->cond.unlock();
            break;
        }
        Log log = self->take();
        self->cond.unlock();
        self->consume(log);
    }
    return nullptr;
}

Log AsyncLog::take()
{
    Log log = logqueue.front();
    logqueue.pop();
    return log;
}

void AsyncLog::consume(Log log)
{
    this->checkLogName();
    if (log.level >= level) {
        const char *time = now().toDefault();
        pthread_t id = log.id;
        const char *sourcefile = log.sourcefile;
        int line = log.line;
        const char *msg = log.msg.c_str();
        switch (log.level) {
        case Logger::DEBUG:
            std::fprintf(lf, "[DEBUG] %s ThreadID:%ld %s:%d: %s\n", time, id, sourcefile, line, msg);
            break;
        case Logger::INFO:
            std::fprintf(lf, "[INFO ] %s ThreadID:%ld %s:%d: %s\n", time, id, sourcefile, line, msg);
            break;
        case Logger::WARN:
            std::fprintf(lf, "[WARN ] %s ThreadID:%ld %s:%d: %s\n", time, id, sourcefile, line, msg);
            break;
        case Logger::ERROR:
            std::fprintf(lf, "[ERROR] %s ThreadID:%ld %s:%d: %s\n", time, id, sourcefile, line, msg);
            break;
        case Logger::FATAL:
            std::fprintf(lf, "[FATAL] %s ThreadID:%ld %s:%d: %s\n", time, id, sourcefile, line, msg);
            break;
        }
    }
}

void AsyncLog::checkLogName()
{
    if (tofile) {
        const char *date = now().toDate();
        if (std::strcmp(currdate.c_str(), date) != 0) {
            if (std::fclose(lf) != 0) {
                ERROR(std::strerror(errno));
            }
            currdate = date;
            std::string lfname = dir;
            lfname += currdate + ".log";
            lf = std::fopen(lfname.c_str(), "a");
        }
    }
}

void AsyncLog::setOutput(Logger::Output output)
{
    switch (output) {
    case Logger::FILE:
        if (!tofile) {
            std::string lfname = dir;
            lfname += currdate + ".log";
            lf = std::fopen(lfname.c_str(), "a");
            tofile = !tofile;
        }
        break;
    case Logger::CONSOLE:
        if (tofile) {
            if (std::fclose(lf) != 0) {
                ERROR(std::strerror(errno));
            }
            lf = stdout;
            tofile = !tofile;
        }
        break;
    }
}

void AsyncLog::addLog(Logger::LogLevel _level, pthread_t _id, const char *sourcefile, int line, const char *fmt, ...)
{
    std::va_list args1, args2;
    va_start(args1, fmt);
    va_copy(args2, args1);
    int size = std::vsnprintf(nullptr, 0, fmt, args1);
    va_end(args1);
    std::vector<char> buf(size + 1);
    std::vsprintf(buf.data(), fmt, args2);
    va_end(args2);
    Log log{ _level, _id, sourcefile, line, std::string(buf.data()) };
    cond.lock();
    logqueue.push(log);
    cond.unlock();
    cond.notify();
}
