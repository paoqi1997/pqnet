#include <cerrno>
#include <cstdarg>
#include <cstring>
#include <sstream>
#include <string>
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
    : level(Logger::INFO), dir("./log/"),
      currdate(now().toDate()), tofile(false), running(true),
      thd(new std::thread([this]{
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
      }))
{
    lf = stdout;
    int res = makeDir(dir);
    if (res != 0) {
        ERROR(getErrorMsg(res));
    }
}

AsyncLog::~AsyncLog()
{
    running = false;
    cond.notify_one();
    if (thd->joinable()) {
        thd->join();
    }
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
        std::ostringstream oss;
        oss << log.id;
        std::size_t id = std::stoll(oss.str());
        const char *time = now().toDefault();
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

void AsyncLog::addLog(
    Logger::LogLevel _level, std::thread::id _id, const char *sourcefile, int line, const char *fmt, ...)
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
    if (true) {
        std::lock_guard<std::mutex> lk(mtx);
        logqueue.push(log);
    }
    cond.notify_one();
}
