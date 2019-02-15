#include <iostream>

#include "../signal.h"
#include "../thread.h"
#include "../threadpool.h"

static bool done = false;

void sighandler(int signo) {
    done = !done;
}

void* func(void *arg) {
    auto self = static_cast<pqnet::Thread*>(arg);
    auto pool = self->getPool();
    const char *name = "pqnet";
    TS_INFO(pool->al, "Hello %s!", name);
    ALOG_INFO(pool->al, "Hello %s!", name);
}

int main()
{
    pqnet::ThreadPool pool(2, func);
    pool.run();
    pqnet::Signal sig(sighandler);
    sig.addSignal(SIGINT);
    sig.addSignal(SIGTERM);
    sig.waitSig();
    for ( ; ; ) {
        if (done && pool.isIdle()) {
            pool.shutdown();
            break;
        }
    }
    std::cout << "Bye!" << std::endl;
    return 0;
}
