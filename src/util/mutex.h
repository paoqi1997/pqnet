#ifndef PQNET_UTIL_MUTEX_H
#define PQNET_UTIL_MUTEX_H

#include <pthread.h>

namespace pqnet
{

class Mutex
{
public:
    Mutex();
    ~Mutex();
    void lock();
    void unlock();
    pthread_mutex_t* getPtr() { return &mutex; }
private:
    pthread_mutex_t mutex;
};

} // namespace pqnet

#endif // PQNET_UTIL_MUTEX_H
