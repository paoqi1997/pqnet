#include <chrono>

#include "timermanager.h"

using std::chrono::system_clock;
using std::chrono::time_point;
using std::chrono::time_point_cast;
using us_type = std::chrono::microseconds;

std::uint64_t now()
{
    time_point<system_clock, us_type> tp = time_point_cast<us_type>(system_clock::now());
    return tp.time_since_epoch().count();
}

TimerNode::TimerNode()
    : timercb(nullptr), arg(nullptr), interval(0), endtime(0), prev(nullptr), next(nullptr)
{

}

TimerNode::TimerNode(const timerCallBack& cb, void *_arg, uint _interval, std::uint64_t _endtime)
    : timercb(cb), arg(_arg), interval(_interval), endtime(_endtime), prev(nullptr), next(nullptr)
{

}

void TimerNode::unlink()
{
    if (prev != nullptr) {
        prev->next = next;
    }
    if (next != nullptr) {
        next->prev = prev;
    }
}

List::List() : head(new TimerNode()), tail(new TimerNode())
{
    head->next = tail;
    tail->prev = head;
}

List::~List()
{
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

void List::push_back(TimerNode *node)
{
    auto p = tail->prev;
    // prev部分
    tail->prev = node;
    node->prev = p;
    // next部分
    p->next = node;
    node->next = tail;
}

TimerId TimerManager::addTimer(const timerCallBack& cb, void *arg, uint expiration, uint interval)
{
    std::uint64_t endtime = jiffies + expiration * 1000;
    auto node = new TimerNode(cb, arg, interval, endtime);
    addTimerNode(node);
    return TimerId(node);
}

void TimerManager::addTimerNode(TimerNode *node)
{
    std::uint64_t endtime = node->endtime;
    uint expiration = endtime - jiffies;
    List *slot;
    // expiration < 2^8
    if (expiration < TVR_SIZE) {
        std::size_t idx = endtime & TVR_MASK;                                // endtime & 2^8-1
        slot = &tw1[idx];
    }
    // expiration < 2^14
    else if (expiration < 1 << (TVR_BITS + TVN_BITS)) {
        std::size_t idx = (endtime >> TVR_BITS) & TVN_MASK;                  // (endtime >> 8) & 2^6-1
        slot = &tw2[idx];
    }
    // expiration < 2^20
    else if (expiration < 1 << (TVR_BITS + 2 * TVN_BITS)) {
        std::size_t idx = (endtime >> (TVR_BITS + TVN_BITS)) & TVN_MASK;     // (endtime >> 14) & 2^6-1
        slot = &tw3[idx];
    }
    // expiration < 2^26
    else if (expiration < 1 << (TVR_BITS + 3 * TVN_BITS)) {
        std::size_t idx = (endtime >> (TVR_BITS + 2 * TVN_BITS)) & TVN_MASK; // (endtime >> 20) & 2^6-1
        slot = &tw4[idx];
    }
    // IDE: Visual Studio 2017
    //     uint expiration = expire
    // if expiration >= 2^26:
    //     then expire is in: [-(2^32 - 2^26), -1] or [2^26, (2^32) - 1]
    // if static_cast<signed long>(expiration) < 0:
    //     then expire is in: [-(2^31), -1] or [2^31, (2^32) - 1]
    else if (static_cast<signed long>(expiration) < 0) {
        std::size_t idx = jiffies & TVR_MASK;                                // jiffies & 2^8-1
        slot = &tw1[idx];
    }
    else {
        std::size_t idx = (endtime >> (TVR_BITS + 3 * TVN_BITS)) & TVN_MASK; // (endtime >> 26) & 2^6-1
        slot = &tw5[idx];
    }
    slot->push_back(node);
}

void TimerManager::delTimer(TimerId tid)
{
    rmNode = tid.getPtr();
    delete rmNode;
}

void TimerManager::handle()
{
    std::uint64_t currJiffies = now();
    while (jiffies <= currJiffies) {
        std::size_t idx = jiffies & TVR_MASK;
        if (!idx && !cascade(tw2, 0) && !cascade(tw3, 1) && !cascade(tw4, 2) && !cascade(tw5, 3)) {
            // Do nothing.
        }
        List *slot = &tw1[idx];
        auto p = slot->begin();
        while (p != slot->end()) {
            auto q = p->next;
            p->run();
            if (p != rmNode) {
                if (p->isPeriodic()) {
                    rmNode = nullptr;
                    p->endtime = jiffies + p->interval * 1000;
                    p->unlink();
                    addTimerNode(p);
                } else {
                    delete p;
                }
            }
            p = q;
        }
        ++jiffies;
    }
}

std::size_t TimerManager::cascade(List *tw, std::size_t n)
{
    std::size_t idx = (jiffies >> (TVR_BITS + n * TVN_BITS)) & TVN_MASK;
    List *slot = &tw[idx];
    auto p = slot->begin();
    while (p != slot->end()) {
        auto q = p->next;
        p->unlink();
        addTimerNode(p);
        p = q;
    }
    return idx;
}
