#include <iostream>

#include "../mutex.h"
#include "../signal.h"
#include "../thread.h"
#include "../threadpool.h"

static pqnet::Mutex mutex;

void* func(void *arg) {
    auto self = static_cast<pqnet::Thread*>(arg);
    auto pool = self->getPool();
    const char *name = "pqnet";
    TS_INFO(mutex, "Hello %s!", name);
    ALOG_INFO(pool->al, "Hello %s!", name);
    return nullptr;
}

int main()
{
    pqnet::ThreadPool pool(2, func);
    auto SIGINT_HANDLER = [&](){
        pool.shutdown();
        std::cout << std::endl;
        std::cout << "Exit thread pool." << std::endl;
    };
    auto SIGTERM_HANDLER = [&](){
        pool.shutdown();
        std::cout << "Exit thread pool." << std::endl;
    };
    pqnet::addSignal(SIGINT, SIGINT_HANDLER);
    pqnet::addSignal(SIGTERM, SIGTERM_HANDLER);
    pqnet::waitSig();
    pool.run();
    return 0;
}
