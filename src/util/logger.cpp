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

Logger::Logger(const char *_sourcefile, int _line, LogLevel _level)
    : sourcefile(_sourcefile),
      line(_line), level(_level)
{
    std::string dir = "./log/";
    if (access(dir.c_str(), F_OK) != 0) {
        if (mkdir(dir.c_str(), 0777) != 0) {
            ERROR(std::strerror(errno));
        }
    }
    std::string lfname = dir;
    lfname += now().toDate();
    lfname += ".log";
    lf = std::fopen(lfname.c_str(), "a");
}

Logger::~Logger()
{
    if (std::fclose(lf) != 0) {
        ERROR(std::strerror(errno));
    }
}

void Logger::log(const char *fmt, ...) const
{
    std::va_list args1, args2;
    va_start(args1, fmt);
    va_copy(args2, args1);
    int size = std::vsnprintf(nullptr, 0, fmt, args1);
    va_end(args1);
    std::vector<char> buf(size + 1);
    std::vsprintf(buf.data(), fmt, args2);
    va_end(args2);
    switch (level) {
    case Logger::TRACE:
        std::fprintf(lf, "[Trace] %s %s:%d: %s\n", now().toDefault(), sourcefile, line, buf.data());
        break;
    case Logger::DEBUG:
        std::fprintf(lf, "[Debug] %s %s:%d: %s\n", now().toDefault(), sourcefile, line, buf.data());
        break;
    case Logger::INFO:
        std::fprintf(lf, "[Info] %s %s:%d: %s\n", now().toDefault(), sourcefile, line, buf.data());
        break;
    case Logger::WARNING:
        std::fprintf(lf, "[Warning] %s %s:%d: %s\n", now().toDefault(), sourcefile, line, buf.data());
        break;
    case Logger::ERROR:
        std::fprintf(lf, "[Error] %s %s:%d: %s\n", now().toDefault(), sourcefile, line, buf.data());
        break;
    case Logger::FATAL:
        std::fprintf(lf, "[Fatal] %s %s:%d: %s\n", now().toDefault(), sourcefile, line, buf.data());
        break;
    default:
        break;
    }
}

void Logger::fastLog(const char *_sourcefile, int _line, LogLevel _level, const char *fmt, ...)
{
    std::va_list args1, args2;
    va_start(args1, fmt);
    va_copy(args2, args1);
    int size = std::vsnprintf(nullptr, 0, fmt, args1);
    va_end(args1);
    std::vector<char> buf(size + 1);
    std::vsprintf(buf.data(), fmt, args2);
    va_end(args2);
    switch (_level) {
    case Logger::TRACE:
        std::fprintf(stdout, "[Trace] %s %s:%d: %s\n", now().toDefault(), _sourcefile, _line, buf.data());
        break;
    case Logger::DEBUG:
        std::fprintf(stdout, "[Debug] %s %s:%d: %s\n", now().toDefault(), _sourcefile, _line, buf.data());
        break;
    case Logger::INFO:
        std::fprintf(stdout, "[Info] %s %s:%d: %s\n", now().toDefault(), _sourcefile, _line, buf.data());
        break;
    case Logger::WARNING:
        std::fprintf(stdout, "[Warning] %s %s:%d: %s\n", now().toDefault(), _sourcefile, _line, buf.data());
        break;
    case Logger::ERROR:
        std::fprintf(stderr, "[Error] %s %s:%d: %s\n", now().toDefault(), _sourcefile, _line, buf.data());
        break;
    case Logger::FATAL:
        std::fprintf(stderr, "[Fatal] %s %s:%d: %s\n", now().toDefault(), _sourcefile, _line, buf.data());
        break;
    default:
        break;
    }
}
