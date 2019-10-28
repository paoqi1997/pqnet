#ifndef COUNT_DOWN_LATCH_H
#define COUNT_DOWN_LATCH_H

#include <atomic>
#include <condition_variable>
#include <mutex>

class CountDownLatch
{
public:
    CountDownLatch(int _count) : count(_count) {}
    void wait() {
        std::unique_lock<std::mutex> lk(mtx);
        cond.wait(lk, [this]{ return !(count > 0); });
    }
    void countDown() {
        if (true) {
            std::lock_guard<std::mutex> lk(mtx);
            --count;
        }
        if (count == 0) {
            cond.notify_all();
        }
    }
    int getCount() const { return count; }
private:
    std::atomic<int> count;
    std::condition_variable cond;
    std::mutex mtx;
};

#endif // COUNT_DOWN_LATCH_H
