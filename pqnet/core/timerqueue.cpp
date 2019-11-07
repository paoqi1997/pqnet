#include <cerrno>
#include <cstring>

#include <sys/timerfd.h>
#include <unistd.h>

#include "../util/logger.h"
#include "../util/timestamp.h"
#include "timerqueue.h"

using namespace pqnet;

TimerQueue::TimerQueue()
{
    tmfd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK);
    if (tmfd == -1) {
        ERROR(std::strerror(errno));
    }
}

TimerQueue::~TimerQueue()
{
    if (close(tmfd) == -1) {
        ERROR(std::strerror(errno));
    }
}

TimerId TimerQueue::addTimer(const timerCallBack& cb, void *arg, uint _expiration, uint _interval)
{
    Timer timer(cb, arg, _interval);
    auto expiration = ms2SecAndNsec(_expiration);
    auto interval = ms2SecAndNsec(_interval);
    std::uint64_t endtime = now().Int16() + _expiration * 1000;
    if (tmqueue.empty()) {
        // 队列为空
        struct itimerspec its;
        its.it_value.tv_sec = expiration.first;
        its.it_value.tv_nsec = expiration.second;
        its.it_interval.tv_sec = interval.first;
        its.it_interval.tv_nsec = interval.second;
        if (timerfd_settime(tmfd, 0, &its, nullptr) == -1) {
            ERROR(std::strerror(errno));
        }
        // std::pair<iterator, bool>
        auto result = tmqueue.insert(std::make_pair(endtime, timer));
        auto it = result.first;
        return it->second.Id();
    } else {
        // 队列不为空
        auto head = tmqueue.begin();
        if (endtime < head->first) {
            // 新加入的定时器最先到期
            struct itimerspec its;
            its.it_value.tv_sec = expiration.first;
            its.it_value.tv_nsec = expiration.second;
            its.it_interval.tv_sec = interval.first;
            its.it_interval.tv_nsec = interval.second;
            if (timerfd_settime(tmfd, 0, &its, nullptr) == -1) {
                ERROR(std::strerror(errno));
            }
        }
        // std::pair<iterator, bool>
        auto result = tmqueue.insert(std::make_pair(endtime, timer));
        auto it = result.first;
        return it->second.Id();
    }
}

void TimerQueue::delTimer(TimerId id)
{
    for (auto it = tmqueue.begin(); it != tmqueue.end(); ++it) {
        if (it->second.Id() == id) {
            // 要删除的定时器最先到期
            if (it == tmqueue.begin()) {
                if (tmqueue.size() == 1) {
                    // 只有一个定时器
                    struct itimerspec its;
                    std::memset(&its, 0, sizeof(its));
                    if (timerfd_settime(tmfd, 0, &its, nullptr) == -1) {
                        ERROR(std::strerror(errno));
                    }
                } else {
                    // 存在两个定时器或以上
                    auto _it = std::next(it, 1);
                    // 更新到期时间
                    uint _diffval = _it->first - now().Int16();
                    auto diffval = us2SecAndNsec(_diffval);
                    // 更新间隔
                    uint _interval = _it->second.Interval();
                    auto interval = ms2SecAndNsec(_interval);
                    struct itimerspec its;
                    its.it_value.tv_sec = diffval.first;
                    its.it_value.tv_nsec = diffval.second;
                    its.it_interval.tv_sec = interval.first;
                    its.it_interval.tv_nsec = interval.second;
                    if (timerfd_settime(tmfd, 0, &its, nullptr) == -1) {
                        ERROR(std::strerror(errno));
                    }
                }
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
            this->refresh();
            Timer timer = it->second;
            timer.run();
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
            ++it;
        }
    }
    if (tmqueue.empty()) {
        // 置空
        struct itimerspec its;
        std::memset(&its, 0, sizeof(its));
        if (timerfd_settime(tmfd, 0, &its, nullptr) == -1) {
            ERROR(std::strerror(errno));
        }
    } else {
        // 重新设置
        auto head = tmqueue.begin();
        // 更新到期时间
        uint _diffval = head->first - currtime;
        auto diffval = us2SecAndNsec(_diffval);
        // 更新间隔
        uint _interval = head->second.Interval();
        auto interval = ms2SecAndNsec(_interval);
        struct itimerspec its;
        its.it_value.tv_sec = diffval.first;
        its.it_value.tv_nsec = diffval.second;
        its.it_interval.tv_sec = interval.first;
        its.it_interval.tv_nsec = interval.second;
        if (timerfd_settime(tmfd, 0, &its, nullptr) == -1) {
            ERROR(std::strerror(errno));
        }
    }
}

void TimerQueue::refresh()
{
    std::uint64_t msg;
    if (read(tmfd, &msg, sizeof(std::uint64_t)) == -1) {
        ERROR(std::strerror(errno));
    }
}
