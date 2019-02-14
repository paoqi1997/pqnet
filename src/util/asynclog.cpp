#include <cerrno>
#include <cstdio>
#include <cstring>

#include <sys/stat.h>
#include <unistd.h>

#include "asynclog.h"
#include "logger.h"
#include "threadpool.h"

using namespace pqnet;

AsyncLog::AsyncLog(ThreadPool *_poolptr) : poolptr(_poolptr), dir("./log/"), currdate(now().toDate())
{
    if (access(dir.c_str(), F_OK) != 0) {
        if (mkdir(dir.c_str(), 0777) != 0) {
            ERROR(std::strerror(errno));
        }
    }
    std::string lfname = dir;
    lfname += currdate;
    lfname += ".log";
    lf = std::fopen(lfname.c_str(), "a");
}

AsyncLog::~AsyncLog()
{
    if (std::fclose(lf) != 0) {
        ERROR(std::strerror(errno));
    }
}

void AsyncLog::run()
{
    if (pthread_create(&id, nullptr, routine, this) != 0) {
        ERROR(std::strerror(errno));
    }
}

void* AsyncLog::routine(void *arg)
{
    auto self = static_cast<AsyncLog*>(arg);
    auto pool = self->getPool();
    for ( ; ; ) {
        self->cond.lock();
        while (pool->isRunning() && self->isIdle()) {
            self->cond.wait();
        }
        if (!pool->isRunning()) {
            self->cond.unlock();
            break;
        }
        auto lmsg = self->take();
        self->cond.unlock();
        self->consume(lmsg);
    }
    return nullptr;
}

LogMsg AsyncLog::take()
{
    auto lmsg = msgqueue.front();
    msgqueue.pop();
    return lmsg;
}

void AsyncLog::consume(LogMsg lmsg)
{
    const char *date = now().toDate();
    if (std::strcmp(currdate.c_str(), date) != 0) {
        this->reset(date);
    }
    switch (lmsg.level) {
    case Logger::TRACE:
        fprintf(lf, "[Trace] %s %s:%d: %s\n", now().toDefault(), lmsg.sourcefile, lmsg.line, lmsg.msg);
        break;
    case Logger::DEBUG:
        fprintf(lf, "[Debug] %s %s:%d: %s\n", now().toDefault(), lmsg.sourcefile, lmsg.line, lmsg.msg);
        break;
    case Logger::INFO:
        fprintf(lf, "[Info] %s %s:%d: %s\n", now().toDefault(), lmsg.sourcefile, lmsg.line, lmsg.msg);
        break;
    case Logger::WARNING:
        fprintf(lf, "[Warning] %s %s:%d: %s\n", now().toDefault(), lmsg.sourcefile, lmsg.line, lmsg.msg);
        break;
    case Logger::ERROR:
        fprintf(lf, "[Error] %s %s:%d: %s\n", now().toDefault(), lmsg.sourcefile, lmsg.line, lmsg.msg);
        break;
    case Logger::FATAL:
        fprintf(lf, "[Fatal] %s %s:%d: %s\n", now().toDefault(), lmsg.sourcefile, lmsg.line, lmsg.msg);
        break;
    default:
        break;
    }
}

void AsyncLog::reset(const char *date)
{
    if (std::fclose(lf) != 0) {
        ERROR(std::strerror(errno));
    }
    currdate = date;
    std::string lfname = dir;
    lfname += currdate;
    lfname += ".log";
    lf = std::fopen(lfname.c_str(), "a");
}

void AsyncLog::pushMsg(const char *sourcefile, int line, Logger::LogLevel level, const char *msg)
{
    LogMsg lmsg{ sourcefile, line, level, msg };
    cond.lock();
    msgqueue.push(lmsg);
    cond.unlock();
    cond.notify();
}
