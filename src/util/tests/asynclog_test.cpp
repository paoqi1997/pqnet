#include <iostream>

#include "../asynclog.h"
#include "../signal.h"
#include "../thread.h"
#include "../threadpool.h"

void* func(void *arg) {
    auto self = static_cast<pqnet::Thread*>(arg);
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
    return 0;
}
