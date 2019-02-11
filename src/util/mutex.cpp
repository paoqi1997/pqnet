#include <cerrno>
#include <cstring>

#include "logger.h"
#include "mutex.h"

namespace pqnet
{

Mutex::Mutex()
{
    if (pthread_mutex_init(&mutex, nullptr) != 0) {
        ERROR(std::strerror(errno));
    }
}

Mutex::~Mutex()
{
    if (pthread_mutex_destroy(&mutex) != 0) {
        ERROR(std::strerror(errno));
    }
}

void Mutex::lock()
{
    if (pthread_mutex_lock(&mutex) != 0) {
        ERROR(std::strerror(errno));
    }
}

void Mutex::unlock()
{
    if (pthread_mutex_unlock(&mutex) != 0) {
        ERROR(std::strerror(errno));
    }
}

} // namespace pqnet
