#include "../util/timestamp.h"
#include "timerqueue.h"

using namespace pqnet;

TimerId TimerQueue::addTimer(const timerCallBack& cb, void *arg, uint _expiration, uint _interval)
{
    Timer timer(cb, arg, _interval);
    // _expiration为0时将其调整为0.1ms
    std::uint64_t endtime = now().Int16() + (_expiration > 0 ? _expiration * 1000 : 100);
    // std::pair<iterator, bool>
    auto result = tmqueue.insert(std::make_pair(endtime, timer));
    auto it = result.first;
    return it->second.Id();
}

void TimerQueue::delTimer(TimerId id)
{
    for (auto it = tmqueue.begin(); it != tmqueue.end(); ++it) {
        if (it->second.Id() == id) {
            // 要删除的定时器最先到期
            if (it == tmqueue.begin()) {
                rhTimerId = id;
            }
            tmqueue.erase(it);
            break;
        }
    }
}

void TimerQueue::handle()
{
    std::uint64_t currtime = now().Int16();
    for (auto it = tmqueue.begin(); it != tmqueue.end(); ) {
        // 已到期
        if (it->first <= currtime) {
            auto tmpIt = std::next(it, 1);
            Timer timer = it->second;
            timer.run();
            // timer 在 run.delTimer 中被移除
            if (rhTimerId == timer.Id()) {
                rhTimerId = 0;
                it = tmpIt;
                continue;
            }
            if (timer.isPeriodic()) {
                // 周期性定时器
                std::uint64_t endtime = currtime + timer.Interval() * 1000;
                it = tmqueue.erase(it);
                tmqueue.insert(std::make_pair(endtime, timer));
            } else {
                // 一次性定时器
                it = tmqueue.erase(it);
            }
        } else {
            break;
        }
    }
}

uint TimerQueue::getExpire() const
{
    if (!tmqueue.empty()) {
        auto head = tmqueue.begin();
        std::uint64_t endtime = head->first / 1000;
        std::uint64_t currtime = now().Int13();
        return endtime >= currtime ? endtime - currtime : Expire;
    } else {
        return Expire;
    }
}
