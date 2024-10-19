#include <chrono>
#include <cstdio>
#include <thread>
#include <vector>

#include "lockFreeQueue.h"

int main()
{
    LockFreeQueue<std::size_t, 5> q;

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    auto producerFunc = [&q](std::size_t tid, std::size_t counter){
        for (;;) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));

            std::size_t counterNext = counter + 2;

            if (counterNext > 30) {
                break;
            }

            bool succ = q.push(counterNext);

            if (succ) {
                counter = counterNext;
                std::printf("p(%d): %d\n", tid, counterNext);
            }
        }
    };

    producers.emplace_back(producerFunc, 1, -1);
    producers.emplace_back(producerFunc, 2, 0);

    for (std::size_t i = 0; i < 2; ++i) {
        consumers.emplace_back([&q](std::size_t tid){
            std::size_t failCount = 0;

            for (;;) {
                std::this_thread::sleep_for(std::chrono::milliseconds(400));

                std::size_t cnt;
                bool succ = q.pop(cnt);

                if (succ) {
                    failCount = 0;
                    std::printf("c(%d): %d\n", tid, cnt);
                } else {
                    ++failCount;
                    std::printf("c(%d).fail: %d\n", tid, failCount);
                }

                if (failCount >= 3) {
                    break;
                }
            }
        }, i + 1);
    }

    for (auto& producer : producers) {
        producer.join();
    }
    for (auto& consumer : consumers) {
        consumer.join();
    }

    return 0;
}
