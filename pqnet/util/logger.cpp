#include <cerrno>
#include <cstdarg>
#include <cstring>
#include <string>
#include <vector>

#include <sys/stat.h>
#include <unistd.h>

#include "logger.h"
#include "timestamp.h"

using namespace pqnet;

Logger *Logger::instance = nullptr;
Logger::Garbo Logger::garbo;

Logger::Logger() : level(Logger::INFO), dir("./log/"), currdate(now().toDate()), tofile(false)
{
    lf = stdout;
    if (access(dir.c_str(), F_OK) != 0) {
        if (errno == ENOENT) {
            if (mkdir(dir.c_str(), 0777) != 0) {
                ERROR(std::strerror(errno));
            }
        } else {
            ERROR(std::strerror(errno));
        }
    }
}

void Logger::checkLogName()
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

void Logger::setOutput(Output output)
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

void Logger::log(LogLevel _level, const char *sourcefile, int line, const char *fmt, ...)
{
    this->checkLogName();
    std::va_list args1, args2;
    va_start(args1, fmt);
    va_copy(args2, args1);
    int size = std::vsnprintf(nullptr, 0, fmt, args1);
    va_end(args1);
    std::vector<char> buf(size + 1);
    std::vsprintf(buf.data(), fmt, args2);
    va_end(args2);
    if (_level >= level) {
        const char *time = now().toDefault();
        switch (_level) {
        case Logger::TRACE:
            std::fprintf(lf, "[Trace  ] %s %s:%d: %s\n", time, sourcefile, line, buf.data());
            break;
        case Logger::DEBUG:
            std::fprintf(lf, "[Debug  ] %s %s:%d: %s\n", time, sourcefile, line, buf.data());
            break;
        case Logger::INFO:
            std::fprintf(lf, "[Info   ] %s %s:%d: %s\n", time, sourcefile, line, buf.data());
            break;
        case Logger::WARNING:
            std::fprintf(lf, "[Warning] %s %s:%d: %s\n", time, sourcefile, line, buf.data());
            break;
        case Logger::ERROR:
            std::fprintf(lf, "[Error  ] %s %s:%d: %s\n", time, sourcefile, line, buf.data());
            break;
        case Logger::FATAL:
            std::fprintf(lf, "[Fatal  ] %s %s:%d: %s\n", time, sourcefile, line, buf.data());
            break;
        }
    }
}
