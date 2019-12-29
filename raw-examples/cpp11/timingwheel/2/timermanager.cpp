#include "timermanager.h"

#include <chrono>
#include <iostream>

using std::chrono::seconds;
using std::chrono::system_clock;
using std::chrono::time_point;
using std::chrono::time_point_cast;
using ms_type = std::chrono::milliseconds;
using us_type = std::chrono::microseconds;

std::uint64_t now()
{
    //return std::chrono::system_clock::now().time_since_epoch().count();
    time_point<system_clock, us_type> tp = time_point_cast<us_type>(system_clock::now());
    return tp.time_since_epoch().count();
}

TimerNode* TimerManager::addTimer(const timerCallBack& cb, void *arg, uint expiration, uint interval)
{
    std::uint64_t expires = expiration * 1000;
    std::uint64_t endtime = jiffies + expires;
    auto node = new TimerNode(cb, arg, interval, endtime);
    addTimerNode(node);
    return node;
}

void TimerManager::addTimerNode(TimerNode *node)
{
    std::uint64_t endtime = node->endtime;
    uint expiration = endtime - jiffies;
    std::size_t i;
    List *list;
    if (expiration < TVR_SIZE) {
        i = endtime & TVR_MASK;
        wheel = 1, slotIdx = i;
        list = &tw1[i];
    } else if (expiration < 1 << (TVR_BITS + TVN_BITS)) {
        i = (endtime >> TVR_BITS) & TVN_MASK;
        wheel = 2, slotIdx = i;
        list = &tw2[i];
    } else if (expiration < 1 << (TVR_BITS + 2 * TVN_BITS)) {
        i = (endtime >> (TVR_BITS + TVN_BITS)) & TVN_MASK;
        wheel = 3, slotIdx = i;
        list = &tw3[i];
    } else if (expiration < 1 << (TVR_BITS + 3 * TVN_BITS)) {
        i = (endtime >> (TVR_BITS + 2 * TVN_BITS)) & TVN_MASK;
        wheel = 4, slotIdx = i;
        list = &tw4[i];
    } else {
        if (expiration > UINT32_MAX) {
            expiration = UINT32_MAX;
        }
        i = (endtime >> (TVR_BITS + 3 * TVN_BITS)) & TVN_MASK;
        wheel = 5, slotIdx = i;
        list = &tw5[i];
    }
    list->push_back(node);
}

void TimerManager::delTimer(TimerNode *node)
{
    delete node;
    if (node) {
        std::cout << "Yes!\n";
    } else {
        std::cout << "No!\n";
    }
    std::cout << node << std::endl;
}

void TimerManager::handle()
{
    std::uint64_t currJiffies = now();
    while (jiffies <= currJiffies) {
        std::size_t idx = jiffies & TVR_MASK;
        if (!idx && !cascade(tw2, 0) && !cascade(tw3, 1) && !cascade(tw4, 2) && !cascade(tw5, 3)) {
            // Do nothing.
        }
        List *list = &tw1[idx];
        auto p = list->begin();
        while (p != list->end()) {
            auto q = p->next;
            p->run();
            std::cout << p << std::endl;
            if (p != nullptr && p->isPeriodic()) {
                p->endtime = jiffies + p->interval * 1000;
                p->disconnect();
                addTimerNode(p);
            } else {
                delete p;
            }
            p = q;
        }
        ++jiffies;
    }
}

std::size_t TimerManager::cascade(List *list, std::size_t n)
{
    std::size_t idx = (jiffies >> (TVR_BITS + n * TVN_BITS)) & TVN_MASK;
    List *slot = &list[idx];
    auto p = slot->begin();
    while (p != slot->end()) {
        auto q = p->next;
        p->disconnect();
        addTimerNode(p);
        p = q;
    }
    return idx;
}
