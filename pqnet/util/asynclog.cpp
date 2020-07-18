#include <cerrno>
#include <cstdarg>
#include <cstring>
#include <string>
#include <utility>
#include <vector>

#include "../platform/base.h"
#include "asynclog.h"
#include "timestamp.h"
#include "types.h"

using namespace pqnet;

std::mutex AsyncLog::mtx;
AsyncLog *AsyncLog::instance = nullptr;
AsyncLog::Garbo AsyncLog::garbo;

AsyncLog::AsyncLog()
    : level(Logger::INFO), target(Logger::CONSOLE),
      dir("./log/"), currdate(now().toDate()), running(true)
{
    lf = stdout;
    int res = makeDir(dir);
    if (res != 0) {
        ERROR(getErrorMsg(res));
    }

    std::unique_ptr<std::thread> tmpthd(
        new std::thread([this]{
            for (;;) {
                Log log;
                if (true) {
                    std::unique_lock<std::mutex> lk(mtx);
                    cond.wait(lk, [this]{ return !this->isRunning() || !this->isIdle(); });
                    if (!this->isRunning() && this->isIdle()) {
                        break;
                    }
                    log = this->take();
                }
                this->consume(log);
            }
        })
    );
    thd = std::move(tmpthd);
}

AsyncLog::~AsyncLog()
{
    running = false;
    cond.notify_one();
    if (thd->joinable()) {
        thd->join();
    }
}

void AsyncLog::checkLogName()
{
    if (target == Logger::FILE) {
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

void AsyncLog::setTarget(Logger::Target _target)
{
    switch (_target) {
    case Logger::FILE:
        if (target == Logger::CONSOLE) {
            std::string lfname = dir;
            lfname += currdate + ".log";
            lf = std::fopen(lfname.c_str(), "a");
            target = _target;
        }
        break;
    case Logger::CONSOLE:
        if (target == Logger::FILE) {
            if (std::fclose(lf) != 0) {
                ERROR(std::strerror(errno));
            }
            lf = stdout;
            target = _target;
        }
        break;
    }
}

void AsyncLog::addLog(
    Logger::LogLevel _level, std::thread::id tid, const char *sourcefile, int line, const char *fmt, ...)
{
    std::va_list args1, args2;
    va_start(args1, fmt);
    va_copy(args2, args1);
    int size = std::vsnprintf(nullptr, 0, fmt, args1);
    va_end(args1);
    std::vector<char> buf(size + 1);
    std::vsprintf(buf.data(), fmt, args2);
    va_end(args2);

    if (true) {
        std::lock_guard<std::mutex> lk(mtx);
        logqueue.emplace(_level, tid, sourcefile, line, buf.data());
    }

    cond.notify_one();
}

void AsyncLog::consume(Log log)
{
    if (log.level < level)
        return;

    checkLogName();

    const char *time = now().toDefault();
    std::size_t id = tid2u64(log.id);
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
