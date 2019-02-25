#include <iostream>

#include "../signal.h"
#include "../threadpool.h"

static bool done = false;

void sighandler(int signo) {
    done = !done;
}

void func(void *arg) {
    std::cout << static_cast<char*>(arg) << std::endl;
}

int main()
{
    pqnet::ThreadPool pool(2);
    pool.run();
    for (int i = 0; i < 10; ++i) {
        pool.addTask(pqnet::Task{ func, const_cast<char*>("Hello pqnet!") });
    }
    pqnet::Signal sig;
    sig.addSignal(SIGINT, sighandler);
    sig.addSignal(SIGTERM, sighandler);
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
