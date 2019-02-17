#include <cerrno>
#include <cstring>

#include <pthread.h>

#include "../util/logger.h"
#include "looperpool.h"

using namespace pqnet;

LooperPool::LooperPool(std::size_t looperNumber) : ln(looperNumber)
{
    for (std::size_t i = 0; i < ln; ++i) {
        pool.emplace_back(new Looper(this));
    }
}

LooperPool::LooperPool(std::size_t looperNumber, pn_thread_func func) : ln(looperNumber)
{
    for (std::size_t i = 0; i < ln; ++i) {
        pool.emplace_back(new Looper(this, func));
    }
}

LooperPool::~LooperPool()
{

}

void LooperPool::run()
{
    for (auto &t : pool) {
        t->setConnectCallBack(conncb);
        t->setCloseCallBack(closecb);
        t->setReadCallBack(readcb);
        t->setMessageCallBack(msgcb);
        t->run();
    }
}

void LooperPool::shutdown()
{
    for (auto &t : pool) {
        if (pthread_join(t->getId(), nullptr) != 0) {
            ERROR(std::strerror(errno));
        }
    }
}
