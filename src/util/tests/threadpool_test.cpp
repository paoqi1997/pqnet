#include <iostream>

#include "../signal.h"
#include "../threadpool.h"

static bool done = false;

void sighandler(int signo) {
    done = !done;
}

void* func(void *arg) {
    std::cout << static_cast<char*>(arg) << std::endl;
}

int main()
{
    pqnet::ThreadPool pool(2);
    pool.start();
    for (int i = 0; i < 10; ++i) {
        pool.addTask(pqnet::Task{ func, const_cast<char*>("Hello pqnet!") });
    }
    pqnet::Signal sig(sighandler);
    sig.addSignal(SIGINT);
    sig.addSignal(SIGTERM);
    sig.waitSig();
    for ( ; ; ) {
        if (done && pool.isEmpty()) {
            pool.stop();
            break;
        }
    }
    std::cout << "Bye!" << std::endl;
    return 0;
}
