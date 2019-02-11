#ifndef PQNET_UTIL_CONDITION_H
#define PQNET_UTIL_CONDITION_H

#include <pthread.h>

#include "mutex.h"

namespace pqnet
{

class Condition
{
public:
    Condition();
    ~Condition();
    void wait();
    void notify();
    void notifyAll();
    void lock() { mutex.lock(); }
    void unlock() { mutex.unlock(); }
private:
    pthread_cond_t cond;
    Mutex mutex;
};

} // namespace pqnet

#endif // PQNET_UTIL_CONDITION_H
