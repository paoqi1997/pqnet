#ifndef PQNET_UTIL_LOGGER_H
#define PQNET_UTIL_LOGGER_H

#include <cstdio>
#include <cstring>
#include <string>

namespace pqnet
{

#define DEBUG(fmt, ...)                                                        \
{                                                                              \
    auto logger = pqnet::Logger::getInstance();                                \
    logger->log(pqnet::Logger::DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
}

#define INFO(fmt, ...)                                                         \
{                                                                              \
    auto logger = pqnet::Logger::getInstance();                                \
    logger->log(pqnet::Logger::INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__);  \
}

#define WARN(fmt, ...)                                                         \
{                                                                              \
    auto logger = pqnet::Logger::getInstance();                                \
    logger->log(pqnet::Logger::WARN, __FILE__, __LINE__, fmt, ##__VA_ARGS__);  \
}

#define ERROR(fmt, ...)                                                        \
{                                                                              \
    auto logger = pqnet::Logger::getInstance();                                \
    logger->log(pqnet::Logger::ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
}

#define FATAL(fmt, ...)                                                        \
{                                                                              \
    auto logger = pqnet::Logger::getInstance();                                \
    logger->log(pqnet::Logger::FATAL, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
}

class Logger
{
public:
    enum LogLevel {
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL
    };
    enum Target {
        FILE,
        CONSOLE
    };
    static Logger* getInstance() {
        if (instance == nullptr) {
            instance = new Logger();
        }
        return instance;
    }
    void checkLogName();
    void setLogLevel(LogLevel _level) { level = _level; }
    void setTarget(Target _target);
    void log(LogLevel _level, const char *sourcefile, int line, const char *fmt, ...);
private:
    LogLevel level;
    Target target;
    std::string dir;
    std::string currdate;
    std::FILE *lf;
private:
    Logger();
    static Logger *instance;
    // To delete the instance
    class Garbo
    {
    public:
        ~Garbo() {
            auto logger = Logger::instance;
            if (logger) {
                if (logger->target == FILE) {
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
