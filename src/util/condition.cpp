#include <cerrno>
#include <cstring>

#include "condition.h"
#include "logger.h"

using namespace pqnet;

Condition::Condition()
{
    if (pthread_cond_init(&cond, nullptr) != 0) {
        ERROR(std::strerror(errno));
    }
}

Condition::~Condition()
{
    if (pthread_cond_destroy(&cond) != 0) {
        ERROR(std::strerror(errno));
    }
}

void Condition::wait()
{
    if (pthread_cond_wait(&cond, mutex.getMutex()) != 0) {
        ERROR(std::strerror(errno));
    }
}

void Condition::notify()
{
    if (pthread_cond_signal(&cond) != 0) {
        ERROR(std::strerror(errno));
    }
}

void Condition::notifyAll()
{
    if (pthread_cond_broadcast(&cond) != 0) {
        ERROR(std::strerror(errno));
    }
}
