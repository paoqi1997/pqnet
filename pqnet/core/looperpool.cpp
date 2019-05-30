#include <cerrno>
#include <cstring>

#include <pthread.h>

#include "../util/logger.h"
#include "looperpool.h"

using namespace pqnet;

LooperPool::LooperPool(std::size_t looperNumber) : ln(looperNumber)
{
    for (std::size_t i = 0; i < ln; ++i) {
        loopers.emplace_back(new Looper());
    }
}

LooperPool::LooperPool(std::size_t looperNumber, pn_thread_func func) : ln(looperNumber)
{
    for (std::size_t i = 0; i < ln; ++i) {
        loopers.emplace_back(new Looper(func));
    }
}

LooperPool::~LooperPool()
{
    for (auto &lp : loopers) {
        if (pthread_join(lp->getId(), nullptr) != 0) {
            ERROR(std::strerror(errno));
        }
    }
}

void LooperPool::run()
{
    for (auto &lp : loopers) {
        lp->setConnectCallBack(conncb);
        lp->setReadCallBack(readcb);
        lp->setMessageCallBack(msgcb);
        lp->setCloseByPeerCallBack(cpcb);
        lp->setCloseBySockCallBack(cscb);
        lp->run();
    }
}
