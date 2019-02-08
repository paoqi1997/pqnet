#include <cerrno>
#include <cstdio>
#include <cstring>

#include <string>

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

void Logger::notset(const char *msg) const
{
    if (level <= Logger::NOTSET) {
        fprintf(lf, "[Notset] %s %s:%d: %s\n", now().toDefault(), sourcefile, line, msg);
    }
}

void Logger::debug(const char *msg) const
{
    if (level <= Logger::DEBUG) {
        fprintf(lf, "[Debug] %s %s:%d: %s\n", now().toDefault(), sourcefile, line, msg);
    }
}

void Logger::info(const char *msg) const
{
    if (level <= Logger::INFO) {
        fprintf(lf, "[Info] %s %s:%d: %s\n", now().toDefault(), sourcefile, line, msg);
    }
}

void Logger::warning(const char *msg) const
{
    if (level <= Logger::WARNING) {
        fprintf(lf, "[Warning] %s %s:%d: %s\n", now().toDefault(), sourcefile, line, msg);
    }
}

void Logger::error(const char *msg) const
{
    if (level <= Logger::ERROR) {
        fprintf(lf, "[Error] %s %s:%d: %s\n", now().toDefault(), sourcefile, line, msg);
    }
}

void Logger::fatal(const char *msg) const
{
    if (level <= Logger::FATAL) {
        fprintf(lf, "[Fatal] %s %s:%d: %s\n", now().toDefault(), sourcefile, line, msg);
    }
}
