#include <iostream>

#include <unistd.h>

#include <pqnet/util/asynclog.h>
#include <pqnet/util/signal.h>
#include <pqnet/util/threadpool.h>

void func(const char *name) {
    AL_INFO("Hello %s!", name);
}

int main()
{
    pqnet::ThreadPool pool(2);
    for (int i = 0; i < 10; ++i) {
        pool.addTask(func, "pqnet");
    }

    auto SIGINT_HANDLER = [&]{
        pool.shutdown();
        std::cout << std::endl;
        std::cout << "Exit thread pool." << std::endl;
    };
    auto SIGTERM_HANDLER = [&]{
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
