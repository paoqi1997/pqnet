#include <cerrno>
#include <chrono>
#include <cstring>
#include <iostream>

#include <sys/timerfd.h>
#include <unistd.h>

#include "timerqueue.h"

using std::chrono::system_clock;
using std::chrono::time_point;
using std::chrono::time_point_cast;
using us_type = std::chrono::microseconds;

std::uint64_t now()
{
    time_point<system_clock, us_type> tp = time_point_cast<us_type>(system_clock::now());
    return tp.time_since_epoch().count();
}

std::pair<uint, uint> ms2SecAndNsec(uint ms)
{
    uint sec = ms / K1E3;
    uint nsec = (ms - sec * K1E3) * K1E6;
    return std::make_pair(sec, nsec);
}

std::pair<uint, uint> us2SecAndNsec(uint us)
{
    uint sec = us / K1E6;
    uint nsec = (us - sec * K1E6) * K1E3;
    return std::make_pair(sec, nsec);
}

TimerQueue::TimerQueue() : rhTimerId(0)
{
    tmfd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK);
    if (tmfd == -1) {
        std::cout << std::strerror(errno) << std::endl;
    }
}

TimerQueue::~TimerQueue()
{
    if (close(tmfd) == -1) {
        std::cout << std::strerror(errno) << std::endl;
    }
}

TimerId TimerQueue::addTimer(const timerCallBack& cb, void *arg, uint _expiration, uint _interval)
{
    Timer timer(cb, arg, _interval);
    // _expiration为0时将其调整为0.1ms
    auto expiration = _expiration > 0 ? ms2SecAndNsec(_expiration) : us2SecAndNsec(100);
    auto interval = ms2SecAndNsec(_interval);
    std::uint64_t endtime = now() + (_expiration > 0 ? _expiration * 1000 : 100);
    if (tmqueue.empty()) {
        // 队列为空
        struct itimerspec its;
        its.it_value.tv_sec = expiration.first;
        its.it_value.tv_nsec = expiration.second;
        its.it_interval.tv_sec = interval.first;
        its.it_interval.tv_nsec = interval.second;
        if (timerfd_settime(tmfd, 0, &its, nullptr) == -1) {
            std::cout << std::strerror(errno) << std::endl;
        }
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
                std::cout << std::strerror(errno) << std::endl;
            }
        }
    }
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
                if (tmqueue.size() == 1) {
                    // 只有一个定时器
                    struct itimerspec its;
                    std::memset(&its, 0, sizeof(its));
                    if (timerfd_settime(tmfd, 0, &its, nullptr) == -1) {
                        std::cout << std::strerror(errno) << std::endl;
                    }
                } else {
                    // 存在两个定时器或以上
                    auto _it = std::next(it, 1);
                    // 更新到期时间
                    uint _expiration = _it->first - now();
                    auto expiration = us2SecAndNsec(_expiration);
                    // 更新间隔
                    uint _interval = _it->second.Interval();
                    auto interval = ms2SecAndNsec(_interval);
                    struct itimerspec its;
                    its.it_value.tv_sec = expiration.first;
                    its.it_value.tv_nsec = expiration.second;
                    its.it_interval.tv_sec = interval.first;
                    its.it_interval.tv_nsec = interval.second;
                    if (timerfd_settime(tmfd, 0, &its, nullptr) == -1) {
                        std::cout << std::strerror(errno) << std::endl;
                    }
                }
                rhTimerId = id;
            }
            tmqueue.erase(it);
            break;
        }
    }
}

void TimerQueue::handle()
{
    this->refresh();
    std::uint64_t currtime = now();
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
    if (tmqueue.empty()) {
        // 置空
        struct itimerspec its;
        std::memset(&its, 0, sizeof(its));
        if (timerfd_settime(tmfd, 0, &its, nullptr) == -1) {
            std::cout << std::strerror(errno) << std::endl;
        }
    } else {
        // 重新设置
        auto head = tmqueue.begin();
        // 更新到期时间
        uint _expiration = head->first - currtime;
        auto expiration = us2SecAndNsec(_expiration);
        // 更新间隔
        uint _interval = head->second.Interval();
        auto interval = ms2SecAndNsec(_interval);
        struct itimerspec its;
        its.it_value.tv_sec = expiration.first;
        its.it_value.tv_nsec = expiration.second;
        its.it_interval.tv_sec = interval.first;
        its.it_interval.tv_nsec = interval.second;
        if (timerfd_settime(tmfd, 0, &its, nullptr) == -1) {
            std::cout << std::strerror(errno) << std::endl;
        }
    }
}

void TimerQueue::refresh()
{
    std::uint64_t msg;
    if (read(tmfd, &msg, sizeof(std::uint64_t)) == -1) {
        std::cout << std::strerror(errno) << std::endl;
    }
}
