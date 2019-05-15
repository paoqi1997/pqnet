#include <cerrno>
#include <cstdarg>
#include <cstring>

#include <vector>

#include <sys/stat.h>
#include <unistd.h>

#include "asynclog.h"
#include "timestamp.h"

using namespace pqnet;

AsyncLog::AsyncLog() : dir("./log/"), currdate(now().toDate())
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
    const char *msg = lmsg.msg.c_str();
    switch (lmsg.level) {
    case Logger::TRACE:
        std::fprintf(lf, "[Trace] %s %s:%d: %s\n", now().toDefault(), lmsg.sourcefile, lmsg.line, msg);
        break;
    case Logger::DEBUG:
        std::fprintf(lf, "[Debug] %s %s:%d: %s\n", now().toDefault(), lmsg.sourcefile, lmsg.line, msg);
        break;
    case Logger::INFO:
        std::fprintf(lf, "[Info] %s %s:%d: %s\n", now().toDefault(), lmsg.sourcefile, lmsg.line, msg);
        break;
    case Logger::WARNING:
        std::fprintf(lf, "[Warning] %s %s:%d: %s\n", now().toDefault(), lmsg.sourcefile, lmsg.line, msg);
        break;
    case Logger::ERROR:
        std::fprintf(lf, "[Error] %s %s:%d: %s\n", now().toDefault(), lmsg.sourcefile, lmsg.line, msg);
        break;
    case Logger::FATAL:
        std::fprintf(lf, "[Fatal] %s %s:%d: %s\n", now().toDefault(), lmsg.sourcefile, lmsg.line, msg);
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

void AsyncLog::pushMsg(const char *sourcefile, int line, Logger::LogLevel level, const char *fmt, ...)
{
    std::va_list args1, args2;
    va_start(args1, fmt);
    va_copy(args2, args1);
    int size = std::vsnprintf(nullptr, 0, fmt, args1);
    va_end(args1);
    std::vector<char> buf(size + 1);
    std::vsprintf(buf.data(), fmt, args2);
    va_end(args2);
    LogMsg lmsg{ sourcefile, line, level, std::string(buf.data()) };
    mtx.lock();
    msgqueue.push(lmsg);
    mtx.unlock();
}
