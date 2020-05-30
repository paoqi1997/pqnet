#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <condition_variable>
#include <mutex>

class Semaphore
{
public:
    Semaphore(int _count) : count(_count) {}
    // P原语，即阻塞原语
    void wait() {
        std::unique_lock<std::mutex> lk(mtx);
        if (--count < 0) {
            cond.wait(lk);
        }
    }
    // V原语，即唤醒原语
    void post() {
        std::lock_guard<std::mutex> lk(mtx);
        // count加1后如果 count <= 0 即表示count之前为-1或更小的值
        // 有线程在等待资源，应通知它
        if (++count <= 0) {
            cond.notify_one();
        }
    }
private:
    int count;
    std::condition_variable cond;
    std::mutex mtx;
};

#endif // SEMAPHORE_H
