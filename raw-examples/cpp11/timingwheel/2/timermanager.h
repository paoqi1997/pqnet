#ifndef TIMER_MANAGER_H
#define TIMER_MANAGER_H

#include <cstdint>
#include <functional>

/**
 * 内存较小时可将其置为1
 */
#define CONFIG_BASE_SMALL 0 // TVN_SIZE=64 TVR_SIZE=256

/**
 * TV: timeval N: nervous R: relax
 */
#define TVN_BITS (CONFIG_BASE_SMALL ? 4 : 6)
#define TVR_BITS (CONFIG_BASE_SMALL ? 6 : 8)
#define TVN_SIZE (1 << TVN_BITS)
#define TVR_SIZE (1 << TVR_BITS)
#define TVN_MASK (TVN_SIZE - 1)
#define TVR_MASK (TVR_SIZE - 1)

using uint = unsigned int;
using timerCallBack = std::function<void(void*)>;

/**
 * Microsecond Level
 */
std::uint64_t now();

struct TimerNode
{
    TimerNode();
    TimerNode(const timerCallBack& cb, void *_arg, uint _interval, std::uint64_t _endtime);
    ~TimerNode() { unlink(); }
    void run() { timercb(arg); }
    void unlink();
    bool isPeriodic() const { return interval > 0; }
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
    List();
    ~List();
    void push_back(TimerNode *node);
    TimerNode* begin() { return head->next; }
    TimerNode* end() { return tail; }
private:
    TimerNode *head;
    TimerNode *tail;
};

class TimerId
{
public:
    TimerId() : node(nullptr) {}
    TimerId(TimerNode *_node) : node(_node) {}
    TimerNode* getPtr() { return node; }
private:
    TimerNode *node;
};

class TimerManager
{
public:
    TimerManager() : rmNode(nullptr), jiffies(now()) {}
    // Millisecond Level
    TimerId addTimer(const timerCallBack& cb, void *arg, uint expiration, uint interval = 0);
    void delTimer(TimerId tid);
    void handle();
private:
    void addTimerNode(TimerNode *node);
    std::size_t cascade(List *tw, std::size_t n);
private:
    TimerNode *rmNode;
    std::uint64_t jiffies;
    List tw1[TVR_SIZE];
    List tw2[TVN_SIZE];
    List tw3[TVN_SIZE];
    List tw4[TVN_SIZE];
    List tw5[TVN_SIZE];
};

#endif // TIMER_MANAGER_H
