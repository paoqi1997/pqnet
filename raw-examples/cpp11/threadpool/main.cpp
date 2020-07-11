#include <chrono>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

#include "threadpool.h"

using std::cout;
using std::endl;

std::thread::id func(void *arg) {
    cout << static_cast<char*>(arg) << endl;
    return std::this_thread::get_id();
}

int main()
{
    ThreadPool pool(4);
    std::vector<std::future<std::thread::id>> futures;

    for (int i = 0; i < 10; ++i) {
        futures.emplace_back(
            pool.addTask(func, const_cast<char*>("Hello std::thread!"))
        );
    }
    pool.run();

    while (pool.isRunning()) {
        if (pool.isIdle()) {
            pool.shutdown();
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    for (auto& future : futures) {
        cout << future.get() << endl;
    }

    return 0;
}
