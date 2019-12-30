#include <chrono>

#include "timermanager.h"

using std::chrono::system_clock;
using std::chrono::time_point;
using std::chrono::time_point_cast;
using us_type = std::chrono::microseconds;

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
    node->prev = p;
    tail->prev = node;
    // next部分
    p->next = node;
    node->next = tail;
}

std::uint64_t now()
{
    time_point<system_clock, us_type> tp = time_point_cast<us_type>(system_clock::now());
    return tp.time_since_epoch().count();
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
    if (expiration < TVR_SIZE) {
        std::size_t idx = endtime & TVR_MASK;
        slot = &tw1[idx];
    } else if (expiration < 1 << (TVR_BITS + TVN_BITS)) {
        std::size_t idx = (endtime >> TVR_BITS) & TVN_MASK;
        slot = &tw2[idx];
    } else if (expiration < 1 << (TVR_BITS + 2 * TVN_BITS)) {
        std::size_t idx = (endtime >> (TVR_BITS + TVN_BITS)) & TVN_MASK;
        slot = &tw3[idx];
    } else if (expiration < 1 << (TVR_BITS + 3 * TVN_BITS)) {
        std::size_t idx = (endtime >> (TVR_BITS + 2 * TVN_BITS)) & TVN_MASK;
        slot = &tw4[idx];
    } else {
        if (expiration > UINT32_MAX) {
            expiration = UINT32_MAX;
        }
        std::size_t idx = (endtime >> (TVR_BITS + 3 * TVN_BITS)) & TVN_MASK;
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
