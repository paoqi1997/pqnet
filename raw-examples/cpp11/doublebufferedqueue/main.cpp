#include <chrono>
#include <iostream>
#include <thread>

#include "doublebufferedqueue.h"

using std::cout;
using std::endl;
using std::chrono::system_clock;
using std::chrono::time_point;
using std::chrono::time_point_cast;
using us_type = std::chrono::microseconds;

std::uint64_t now()
{
    time_point<system_clock, us_type> tp = time_point_cast<us_type>(system_clock::now());
    return tp.time_since_epoch().count();
}

int cnt1 = 0, cnt2 = 0;
int sum = 10000;

void producerFunc1(Queue<int> *queue);
void producerFunc2(DoubleBufferedQueue<int> *queue);

void consumerFunc1(Queue<int> *queue);
void consumerFunc2(DoubleBufferedQueue<int> *queue);

int main()
{
    std::uint64_t t1, t2;

    for (int i = 0; i < 5; ++i) {
        // Queue
        {
            Queue<int> queue;

            auto beginTime = now();

            std::thread producer(producerFunc1, &queue);
            std::thread consumer(consumerFunc1, &queue);

            producer.join();
            consumer.join();

            t1 = now() - beginTime;
        }
        // DoubleBufferedQueue
        {
            DoubleBufferedQueue<int> queue;

            auto beginTime = now();

            std::thread producer(producerFunc2, &queue);
            std::thread consumer(consumerFunc2, &queue);

            producer.join();
            consumer.join();

            t2 = now() - beginTime;
        }
        // 输出耗时
        cout << t1 << ' ' << t2 << endl;
    }

    return 0;
}

void producerFunc1(Queue<int> *queue) {
    for (int i = 0; i < sum; ++i) {
        queue->push(i);
    }
}

void producerFunc2(DoubleBufferedQueue<int> *queue) {
    for (int i = 0; i < sum; ++i) {
        queue->push(i);
    }
}

void consumerFunc1(Queue<int> *queue) {
    for (;;) {
        if (cnt1 == sum) {
            cnt1 = 0;
            break;
        } else if (!queue->empty()) {
            queue->pop();
            ++cnt1;
        }
    }
}

void consumerFunc2(DoubleBufferedQueue<int> *queue) {
    for (;;) {
        if (cnt2 == sum) {
            cnt2 = 0;
            break;
        } else if (!queue->empty()) {
            queue->pop();
            ++cnt2;
        }
    }
}
