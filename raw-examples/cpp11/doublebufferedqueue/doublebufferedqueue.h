#ifndef DOUBLE_BUFFERED_QUEUE_H
#define DOUBLE_BUFFERED_QUEUE_H

#include <mutex>
#include <queue>

template <typename T>
class Queue
{
public:
    Queue() = default;
    void push(const T& value) {
        std::lock_guard<std::mutex> lk(mtx);
        queue.push(value);
    }
    T pop() {
        std::lock_guard<std::mutex> lk(mtx);
        T value = queue.front();
        queue.pop();
        return value;
    }
    bool empty() const {
        return queue.empty();
    }
private:
    std::queue<T> queue;
    std::mutex mtx;
};

template <typename T>
class DoubleBufferedQueue
{
public:
    DoubleBufferedQueue() = default;
    void push(const T& value) {
        std::lock_guard<std::mutex> wlk(wmtx);
        wq.push(value);
    }
    T pop() {
        std::lock_guard<std::mutex> rlk(rmtx);
        if (rq.empty() && !wq.empty()) {
            std::lock_guard<std::mutex> wlk(wmtx);
            rq.swap(wq);
        }
        T value = rq.front();
        rq.pop();
        return value;
    }
    bool empty() const {
        return (rq.empty() && wq.empty());
    }
private:
    std::queue<T> rq;
    std::queue<T> wq;
    std::mutex rmtx;
    std::mutex wmtx;
};

#endif // DOUBLE_BUFFERED_QUEUE_H
