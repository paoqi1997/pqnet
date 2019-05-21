#include <iostream>

#include <unistd.h>

#include "../signal.h"
#include "../threadpool.h"

void func(void *arg) {
    std::cout << static_cast<char*>(arg) << std::endl;
}

int main()
{
    pqnet::ThreadPool pool(2);
    for (int i = 0; i < 10; ++i) {
        pool.addTask(pqnet::Task{ func, const_cast<char*>("Hello pqnet!") });
    }
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
