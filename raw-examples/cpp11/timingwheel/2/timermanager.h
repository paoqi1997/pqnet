#ifndef TIMER_MANAGER_H
#define TIMER_MANAGER_H

#include <cstdint>
#include <functional>
#include <string>

#include "list.h"

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

#define BUCKET_BITS (CONFIG_BASE_SMALL ? 6 : 8)
#define BUCKET_SIZE (1 << BUCKET_BITS)
#define BUCKET_MASK (BUCKET_SIZE - 1)

using uint = unsigned int;
using timerCallBack = std::function<void(void*)>;

/**
 * Microsecond Level
 */
std::uint64_t now();

struct TimerNode
{
    TimerNode(
        const std::string& _sID,
        const timerCallBack& cb, void *_arg,
        uint _interval, std::uint64_t _endtime
    );
    void run() { timercb(arg); }
    bool isPeriodic() const { return interval > 0; }
    std::string sID;
    timerCallBack timercb;
    void *arg;
    uint interval;
    std::uint64_t endtime;
    LinkNode<TimerNode*> *htNode;
    LinkNode<TimerNode*> *twNode;
};

class HashTable
{
public:
    HashTable() = default;
    LinkNode<TimerNode*>* push_back(TimerNode *tNode);
    void erase(const std::string& sID);
private:
    std::hash<std::string> hash;
    List<TimerNode*> buckets[BUCKET_SIZE];
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
    TimerId addTimer(
        const std::string& sID,
        const timerCallBack& cb, void *arg,
        uint expiration, uint interval = 0
    );
    void delTimer(TimerId tid);
    void handle();
private:
    LinkNode<TimerNode*>* addTimerNode(TimerNode *node);
    std::size_t cascade(List<TimerNode*> *tw, std::size_t n);
private:
    TimerNode *rmNode;
    std::uint64_t jiffies;
    HashTable htable;
    List<TimerNode*> tw1[TVR_SIZE];
    List<TimerNode*> tw2[TVN_SIZE];
    List<TimerNode*> tw3[TVN_SIZE];
    List<TimerNode*> tw4[TVN_SIZE];
    List<TimerNode*> tw5[TVN_SIZE];
};

#endif // TIMER_MANAGER_H
