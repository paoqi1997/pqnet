#include <chrono>
#include <iostream>

#include "threadpool.h"

void func(void *arg) {
    std::cout << static_cast<char*>(arg) << std::endl;
}

int main()
{
    ThreadPool pool(4);
    for (int i = 0; i < 10; ++i) {
        pool.addTask(Task{ func, const_cast<char*>("Hello std::thread!") });
    }
    pool.run();
    while (pool.isRunning()) {
        if (pool.isIdle()) {
            pool.shutdown();
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    return 0;
}
