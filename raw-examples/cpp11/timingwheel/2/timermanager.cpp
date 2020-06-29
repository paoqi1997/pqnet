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

TimerNode::TimerNode(
    const std::string& _sID,
    const timerCallBack& cb, void *_arg,
    uint _interval, std::uint64_t _endtime
)
    : sID(_sID), timercb(cb), arg(_arg),
      interval(_interval), endtime(_endtime), htNode(nullptr), twNode(nullptr)
{

}

LinkNode<TimerNode*>* HashTable::push_back(TimerNode *tNode)
{
    std::size_t idx = hash(tNode->sID) & BUCKET_MASK;
    return buckets[idx].push_back(tNode);
}

void HashTable::erase(const std::string& sID)
{
    std::size_t nID = hash(sID);
    std::size_t idx = nID & BUCKET_MASK;
    List<TimerNode*> *bucket = &buckets[idx];
    auto p = bucket->begin();
    while (p != bucket->end()) {
        auto q = p->next;
        auto node = p->element;
        if (nID == hash(node->sID) && sID == node->sID) {
            delete node->htNode;
            delete node->twNode;
            delete node;
        }
        p = q;
    }
}

TimerId TimerManager::addTimer(
    const std::string& sID,
    const timerCallBack& cb, void *arg,
    uint expiration, uint interval)
{
    std::uint64_t endtime = jiffies + expiration * 1000;
    auto node = new TimerNode(sID, cb, arg, interval, endtime);
    node->htNode = htable.push_back(node);
    node->twNode = addTimerNode(node);
    return TimerId(node);
}

LinkNode<TimerNode*>* TimerManager::addTimerNode(TimerNode *node)
{
    std::uint64_t endtime = node->endtime;
    uint expiration = endtime - jiffies;
    List<TimerNode*> *slot;
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
    return slot->push_back(node);
}

void TimerManager::delTimer(TimerId tid)
{
    rmNode = tid.getPtr();
    htable.erase(rmNode->sID);
}

void TimerManager::handle()
{
    std::uint64_t currJiffies = now();
    while (jiffies <= currJiffies) {
        std::size_t idx = jiffies & TVR_MASK;
        if (!idx && !cascade(tw2, 0) && !cascade(tw3, 1) && !cascade(tw4, 2) && !cascade(tw5, 3)) {
            // Do nothing.
        }
        List<TimerNode*> *slot = &tw1[idx];
        auto p = slot->begin();
        while (p != slot->end()) {
            auto q = p->next;
            auto node = p->element;
            node->run();
            if (node == rmNode) {
                rmNode = nullptr;
            } else {
                if (node->isPeriodic()) {
                    node->endtime = jiffies + node->interval * 1000;
                    delete p;
                    node->twNode = addTimerNode(node);
                } else {
                    delete node->htNode;
                    delete node->twNode;
                    delete node;
                }
            }
            p = q;
        }
        ++jiffies;
    }
}

std::size_t TimerManager::cascade(List<TimerNode*> *tw, std::size_t n)
{
    std::size_t idx = (jiffies >> (TVR_BITS + n * TVN_BITS)) & TVN_MASK;
    List<TimerNode*> *slot = &tw[idx];
    auto p = slot->begin();
    while (p != slot->end()) {
        auto q = p->next;
        auto node = p->element;
        delete p;
        node->twNode = addTimerNode(node);
        p = q;
    }
    return idx;
}
