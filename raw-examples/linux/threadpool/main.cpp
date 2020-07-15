#include <iostream>

#include <unistd.h>

#include "threadpool.h"

void func(void *arg) {
    std::cout << static_cast<char*>(arg) << std::endl;
}

int main()
{
    ThreadPool pool(2);

    for (int i = 0; i < 10; ++i) {
        pool.addTask(Task{ func, const_cast<char*>("Hello pthread!") });
    }
    pool.run();

    while (pool.isRunning()) {
        if (pool.isIdle()) {
            pool.shutdown();
        } else {
            sleep(1);
        }
    }

    return 0;
}
