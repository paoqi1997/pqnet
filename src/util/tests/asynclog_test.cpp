#include <iostream>

#include "../mutex.h"
#include "../signal.h"
#include "../thread.h"
#include "../threadpool.h"

static bool done = false;

static pqnet::Mutex mutex;

void sighandler(int signo) {
    done = !done;
}

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
    pool.run();
    pqnet::Signal sig;
    sig.addSignal(SIGINT, sighandler);
    sig.addSignal(SIGTERM, sighandler);
    sig.waitSig();
    for ( ; ; ) {
        pool.flush();
        if (done && pool.isIdle()) {
            pool.shutdown();
            break;
        }
    }
    std::cout << "Bye!" << std::endl;
    return 0;
}
