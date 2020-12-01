#include <cerrno>
#include <cstdarg>
#include <cstring>
#include <string>
#include <vector>

#include "../platform/base.h"
#include "defines.h"
#include "logger.h"
#include "timestamp.h"

using namespace pqnet;

Logger *Logger::instance = nullptr;
Logger::Garbo Logger::garbo;

Logger::Logger()
    : level(INFO), target(CONSOLE),
      dir("./log/"), currdate(now().toDate())
{
    lf = stdout;
    int res = makeDir(dir);
    if (res != 0) {
        ERROR(getErrorMsg(res));
    }
}

void Logger::checkDate(const char *date)
{
    if (target == FILE) {
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

void Logger::setTarget(Target _target)
{
    switch (_target) {
    case FILE:
        if (target == CONSOLE) {
            std::string lfname = dir;
            lfname += currdate + ".log";
            lf = std::fopen(lfname.c_str(), "a");
            target = _target;
        }
        break;
    case CONSOLE:
        if (target == FILE) {
            if (std::fclose(lf) != 0) {
                ERROR(std::strerror(errno));
            }
            lf = stdout;
            target = _target;
        }
        break;
    }
}

void Logger::log(LogLevel _level, const char *sourcefile, int line, const char *fmt, ...)
{
    if (_level < level)
        return;

    TimeStamp ts = now();
    checkDate(ts.toDate());

    const char *time = ts.toDefault();

    std::va_list args1, args2;
    va_start(args1, fmt);
    va_copy(args2, args1);
    int size = std::vsnprintf(nullptr, 0, fmt, args1);
    va_end(args1);
    std::vector<char> buf(size + 1);
    std::vsprintf(buf.data(), fmt, args2);
    va_end(args2);

    switch (_level) {
    case Logger::DEBUG:
        std::fprintf(lf, "[DEBUG] %s %s:%d: %s\n", time, sourcefile, line, buf.data());
        break;
    case Logger::INFO:
        std::fprintf(lf, "[INFO ] %s %s:%d: %s\n", time, sourcefile, line, buf.data());
        break;
    case Logger::WARN:
        std::fprintf(lf, "[WARN ] %s %s:%d: %s\n", time, sourcefile, line, buf.data());
        break;
    case Logger::ERROR:
        std::fprintf(lf, "[ERROR] %s %s:%d: %s\n", time, sourcefile, line, buf.data());
        break;
    case Logger::FATAL:
        std::fprintf(lf, "[FATAL] %s %s:%d: %s\n", time, sourcefile, line, buf.data());
        break;
    }
}
