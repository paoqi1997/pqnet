#ifndef TIMER_MANAGER_H
#define TIMER_MANAGER_H

#include <cstdint>
#include <functional>

#define CONFIG_BASE_SMALL 0 // TVN_SIZE=64 TVR_SIZE=256

#define TVN_BITS (CONFIG_BASE_SMALL ? 4 : 6)
#define TVR_BITS (CONFIG_BASE_SMALL ? 6 : 8)
#define TVN_SIZE (1 << TVN_BITS)
#define TVR_SIZE (1 << TVR_BITS)
#define TVN_MASK (TVN_SIZE - 1)
#define TVR_MASK (TVR_SIZE - 1)

#define INDEX(TIME, N) ((TIME >> (TVR_BITS + N * TVN_BITS)) & TVN_MASK)

using uint = unsigned int;
using timerCallBack = std::function<void(void*)>;

std::uint64_t now();

struct TimerNode
{
    TimerNode() : prev(nullptr), next(nullptr) {}
    TimerNode(const timerCallBack& cb, void *_arg, uint _interval, std::uint64_t _endtime)
        : timercb(cb), arg(_arg), interval(_interval), endtime(_endtime), prev(nullptr), next(nullptr) {}
    ~TimerNode() { disconnect(); }
    void run() { timercb(arg); }
    bool isPeriodic() const { return interval > 0; }
    void disconnect() {
        if (prev != nullptr) {
            prev->next = next;
        }
        if (next != nullptr) {
            next->prev = prev;
        }
    }
    timerCallBack timercb;
    void *arg;
    uint interval;
    std::uint64_t endtime;
    TimerNode *prev;
    TimerNode *next;
};

class List
{
public:
    List() : head(new TimerNode()), tail(new TimerNode()) {
        head->next = tail;
        tail->prev = head;
    }
    ~List() {
        auto p = head->next;
        while (p != tail) {
            auto q = p->next;
            delete p;
            p = q;
        }
        delete head;
        delete tail;
        head = tail = nullptr;
    }
    void push_back(TimerNode *node) {
        auto p = tail->prev;
        // prev部分
        node->prev = p;
        tail->prev = node;
        // next部分
        p->next = node;
        node->next = tail;
    }
    TimerNode* begin() { return head->next; }
    TimerNode* end() { return tail; }
private:
    TimerNode *head;
    TimerNode *tail;
};

class TimerManager
{
public:
    TimerManager() : jiffies(now()) {}
    TimerNode* addTimer(const timerCallBack& cb, void *arg, uint expiration, uint interval = 0);
    void delTimer(TimerNode *node);
    void handle();
private:
    void addTimerNode(TimerNode *node);
    std::size_t cascade(List *list, std::size_t n);
private:
    std::size_t wheel;
    std::size_t slotIdx;
    std::uint64_t jiffies;
    List tw1[TVR_SIZE];
    List tw2[TVN_SIZE];
    List tw3[TVN_SIZE];
    List tw4[TVN_SIZE];
    List tw5[TVN_SIZE];
};

#endif // TIMER_MANAGER_H
