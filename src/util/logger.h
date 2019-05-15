#ifndef PQNET_UTIL_LOGGER_H
#define PQNET_UTIL_LOGGER_H

#include <cstdio>
#include <cstring>

#include "types.h"

namespace pqnet
{

#define TRACE(fmt, ...)                                                          \
{                                                                                \
    auto logger = pqnet::Logger::getLogger();                                    \
    logger->log(__FILE__, __LINE__, pqnet::Logger::TRACE, fmt, ##__VA_ARGS__);   \
}

#define DEBUG(fmt, ...)                                                          \
{                                                                                \
    auto logger = pqnet::Logger::getLogger();                                    \
    logger->log(__FILE__, __LINE__, pqnet::Logger::DEBUG, fmt, ##__VA_ARGS__);   \
}

#define INFO(fmt, ...)                                                           \
{                                                                                \
    auto logger = pqnet::Logger::getLogger();                                    \
    logger->log(__FILE__, __LINE__, pqnet::Logger::INFO, fmt, ##__VA_ARGS__);    \
}

#define WARNING(fmt, ...)                                                        \
{                                                                                \
    auto logger = pqnet::Logger::getLogger();                                    \
    logger->log(__FILE__, __LINE__, pqnet::Logger::WARNING, fmt, ##__VA_ARGS__); \
}

#define ERROR(fmt, ...)                                                          \
{                                                                                \
    auto logger = pqnet::Logger::getLogger();                                    \
    logger->log(__FILE__, __LINE__, pqnet::Logger::ERROR, fmt, ##__VA_ARGS__);   \
}

#define FATAL(fmt, ...)                                                          \
{                                                                                \
    auto logger = pqnet::Logger::getLogger();                                    \
    logger->log(__FILE__, __LINE__, pqnet::Logger::FATAL, fmt, ##__VA_ARGS__);   \
}

class Logger
{
public:
    enum LogLevel {
        TRACE,
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };
    enum Output {
        FILE,
        TERMINAL
    };
    static Logger *getLogger() {
        if (instance == nullptr) {
            instance = new Logger();
        }
        return instance;
    }
    void setLevel(LogLevel _level) {
        level = _level;
    }
    void checkDate();
    void setOutput(Output output);
    void log(const char *sourcefile, int line, LogLevel _level, const char *fmt, ...);
private:
    LogLevel level;
    std::string dir;
    std::string currdate;
    bool tofile;
    std::FILE *lf;
    Logger();
    static Logger *instance;
    class Garbo
    {
    public:
        ~Garbo() {
            auto logger = Logger::instance;
            if (logger) {
                if (logger->tofile) {
                    if (std::fclose(logger->lf) != 0) {
                        ERROR(std::strerror(errno));
                    }
                }
                delete logger;
                logger = nullptr;
            }
        }
    };
    static Garbo garbo;
};

} // namespace pqnet

#endif // PQNET_UTIL_LOGGER_H
