#include <iostream>

#include <unistd.h>

#include "../asynclog.h"
#include "../signal.h"
#include "../threadpool.h"

void* func(void *arg) {
    const char *name = "pqnet";
    AL_INFO("Hello %s!", name);
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
    for (;;) {
        if (!pool.isRunning() && pool.isIdle()) {
            break;
        } else {
            sleep(1);
        }
    }
    return 0;
}
