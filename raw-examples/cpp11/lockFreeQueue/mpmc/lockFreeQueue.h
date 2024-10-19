#ifndef LOCK_FREE_QUEUE_H
#define LOCK_FREE_QUEUE_H

#include <atomic>
#include <cstddef>

template <typename T>
class Slot
{
public:
    Slot() : pushFinishedCount(0), popFinishedCount(0) {}
    std::atomic<std::size_t> pushFinishedCount; // 该槽的 push 操作已经完成了多少次
    std::atomic<std::size_t> popFinishedCount;  // 该槽的 pop 操作已经完成了多少次
    T val;
};

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
    Slot<T> _queue[N]; // [nHead, nTail)
};

template <typename T, std::size_t N>
bool LockFreeQueue<T, N>::push(const T& element)
{
    std::size_t tail = nTail.load(std::memory_order_relaxed);

    for (;;) {
        std::size_t index = tail % N;

        Slot<T>& slot = _queue[index];

        std::size_t pushCount = slot.pushFinishedCount.load(std::memory_order_acquire);
        std::size_t popCount = slot.popFinishedCount.load(std::memory_order_relaxed);

        // 相当于该槽已经存放了元素
        if (pushCount > popCount) {
            return false;
        }

        std::size_t round = tail / N; // 相当于第几轮 push 操作，比如 round 为0表示第一轮
        bool isMyTurn = round == pushCount;

        if (isMyTurn) {
            if (nTail.compare_exchange_weak(tail, tail + 1, std::memory_order_relaxed)) {
                slot.val = element;
                slot.pushFinishedCount.store(pushCount + 1, std::memory_order_release);
                return true;
            }
        } else {
            tail = nTail.load(std::memory_order_relaxed);
        }
    }
}

template <typename T, std::size_t N>
bool LockFreeQueue<T, N>::pop(T& element)
{
    std::size_t head = nHead.load(std::memory_order_relaxed);

    for (;;) {
        std::size_t index = head % N;

        Slot<T>& slot = _queue[index];

        std::size_t pushCount = slot.pushFinishedCount.load(std::memory_order_relaxed);
        std::size_t popCount = slot.popFinishedCount.load(std::memory_order_acquire);

        // 相当于该槽没有存放元素
        if (popCount == pushCount) {
            return false;
        }

        std::size_t round = head / N; // 相当于第几轮 pop 操作，比如 round 为0表示第一轮
        bool isMyTurn = round == popCount;

        if (isMyTurn) {
            if (nHead.compare_exchange_weak(head, head + 1, std::memory_order_relaxed)) {
                element = slot.val;
                slot.popFinishedCount.store(popCount + 1, std::memory_order_release);
                return true;
            }
        } else {
            head = nHead.load(std::memory_order_relaxed);
        }
    }
}

#endif // LOCK_FREE_QUEUE_H
