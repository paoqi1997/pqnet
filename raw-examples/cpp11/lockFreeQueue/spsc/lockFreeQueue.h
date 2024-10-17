#ifndef LOCK_FREE_QUEUE_H
#define LOCK_FREE_QUEUE_H

#include <atomic>
#include <cstddef>

template <typename T, std::size_t N>
class LockFreeQueue
{
public:
    LockFreeQueue() : nHead(0), nTail(0) {}
    bool push(const T& element);
    bool pop(T& x);
private:
    std::atomic<std::size_t> nHead; // 队头元素索引
    std::atomic<std::size_t> nTail; // 队尾索引
    T _queue[N]; // [nHead, nTail)
};

// 对于线程来说，应将其视作一个面向队尾的写操作，对应有 nTail.store - release
// 那么另一个线程对同一个原子变量的读操作应为 nTail.load - acquire
template <typename T, std::size_t N>
bool LockFreeQueue<T, N>::push(const T& element)
{
    std::size_t tail = nTail.load(std::memory_order_relaxed);
    std::size_t tailNext = tail + 1;

    // 队尾元素准备塞到末端
    if (tailNext == N) {
        tailNext = 0;
    }

    std::size_t head = nHead.load(std::memory_order_acquire);

    // 检查队列是否还差一个元素就满
    if (tailNext == head) {
        return false;
    }

    _queue[tail] = element;

    nTail.store(tailNext, std::memory_order_release);

    return true;
}

// 对于线程来说，应将其视作一个面向队头的写操作，对应有 nHead.store - release
// 那么另一个线程对同一个原子变量的读操作应为 nHead.load - acquire
template <typename T, std::size_t N>
bool LockFreeQueue<T, N>::pop(T& element)
{
    std::size_t head = nHead.load(std::memory_order_relaxed);
    std::size_t tail = nTail.load(std::memory_order_acquire);

    // 检查队列是否为空
    if (head == tail) {
        return false;
    }

    element = _queue[head];

    ++head;

    // 队头元素已到末端
    if (head == N) {
        head = 0;
    }

    nHead.store(head, std::memory_order_release);

    return true;
}

#endif // LOCK_FREE_QUEUE_H
