#include <iostream>
#include <thread>
#include <vector>

#include "countdownlatch.h"

void func(CountDownLatch *latch) {
    std::cout << "Hello CountDownLatch!" << std::endl;
    latch->countDown();
    std::printf("Count: %d\n", latch->getCount());
}

int main()
{
    const int tCount = 10;
    CountDownLatch cdLatch(tCount);
    std::vector<std::thread> pool;
    for (int i = 0; i < tCount; ++i) {
        pool.emplace_back(func, &cdLatch);
    }
    std::printf("Count1: %d\n", cdLatch.getCount());
    cdLatch.wait();
    std::printf("Count2: %d\n", cdLatch.getCount());
    for (auto& t : pool) {
        if (t.joinable()) {
            t.join();
        }
    }
    return 0;
}
