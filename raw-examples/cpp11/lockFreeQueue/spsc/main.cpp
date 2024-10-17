#include <chrono>
#include <iostream>
#include <thread>

#include "lockFreeQueue.h"

int main()
{
    LockFreeQueue<std::size_t, 5> q;

    std::thread producer([&q](){
        std::size_t counter = 0;

        for (;;) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));

            std::size_t counterNext = counter + 1;
            bool succ = q.push(counterNext);

            if (succ) {
                counter = counterNext;
                std::cout << "p: " << counter << std::endl;
            }
            if (counter >= 10) {
                break;
            }
        }
    });

    std::thread consumer([&q](){
        std::size_t failCount = 0;

        for (;;) {
            std::this_thread::sleep_for(std::chrono::milliseconds(400));

            std::size_t cnt;
            bool succ = q.pop(cnt);

            if (succ) {
                failCount = 0;
                std::cout << "c: " << cnt << std::endl;
            } else {
                ++failCount;
                std::cout << "c.fail: " << failCount << std::endl;
            }

            if (failCount >= 3) {
                break;
            }
        }
    });

    producer.join();
    consumer.join();

    return 0;
}
