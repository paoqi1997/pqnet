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
    auto cb = [](const TcpConnPtr&){};
    if (conncb == nullptr) {
        std::printf("LoopNoConn!\n");
        conncb = cb;
    }
    if (closecb == nullptr) {
        std::printf("LoopNoClose!\n");
        closecb = cb;
    }
    if (readcb == nullptr) {
        std::printf("LoopNoRead!\n");
        readcb = cb;
    }
    if (msgcb == nullptr) {
        std::printf("LoopNoMsg!\n");
        msgcb = cb;
    }
    /*
    auto ptr = std::make_shared<TcpConnection>(233);
    conncb(ptr); */
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
