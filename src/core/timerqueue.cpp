#include <cerrno>
#include <cstring>

#include <utility>

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
    if (timerqueue.empty()) {
        // 队列为空
        struct itimerspec its;
        its.it_value.tv_sec = expiration.first;
        its.it_value.tv_nsec = expiration.second;
        its.it_interval.tv_sec = interval.first;
        its.it_interval.tv_nsec = interval.second;
        if (timerfd_settime(tmfd, 0, &its, nullptr) == -1) {
            ERROR(std::strerror(errno));
        }
        // return std::pair<iterator, bool>
        auto result = timerqueue.insert(std::make_pair(endtime, timer));
        auto it = result.first;
        return it->second.getId();
    } else {
        // 队列不为空
        auto head = timerqueue.begin();
        if (head->first <= endtime) {
            // 稍后到期
            // return std::pair<iterator, bool>
            auto result = timerqueue.insert(std::make_pair(endtime, timer));
            auto it = result.first;
            return it->second.getId();
        } else {
            // 最先到期
            struct itimerspec its;
            its.it_value.tv_sec = expiration.first;
            its.it_value.tv_nsec = expiration.second;
            its.it_interval.tv_sec = interval.first;
            its.it_interval.tv_nsec = interval.second;
            if (timerfd_settime(tmfd, 0, &its, nullptr) == -1) {
                ERROR(std::strerror(errno));
            }
            // return std::pair<iterator, bool>
            auto result = timerqueue.insert(std::make_pair(endtime, timer));
            auto it = result.first;
            return it->second.getId();
        }
    }
}

void TimerQueue::delTimer(TimerId id)
{
    for (auto it = timerqueue.begin(); it != timerqueue.end(); ++it) {
        if (it->second.getId() == id) {
            // 最先到期
            if (it == timerqueue.begin()) {
                if (timerqueue.size() == 1) {
                    // 只有一个定时器
                    struct itimerspec its;
                    std::memset(&its, 0, sizeof(its));
                    if (timerfd_settime(tmfd, 0, &its, nullptr) == -1) {
                        ERROR(std::strerror(errno));
                    }
                } else {
                    // 存在两个定时器或以上
                    uint64_t usec = std::next(it, 1)->first - now().Int16();
                    uint sec = usec / 1000000;
                    uint nsec = (usec - sec * 1000000) * 1000;
                    Timer timer = std::next(it, 1)->second;
                    uint _interval = timer.getInterval();
                    auto interval = ms2SecAndNsec(_interval);
                    struct itimerspec its;
                    its.it_value.tv_sec = sec;
                    its.it_value.tv_nsec = nsec;
                    its.it_interval.tv_sec = interval.first;
                    its.it_interval.tv_nsec = interval.second;
                    if (timerfd_settime(tmfd, 0, &its, nullptr) == -1) {
                        ERROR(std::strerror(errno));
                    }
                }
            }
            timerqueue.erase(it);
            break;
        }
    }
}

void TimerQueue::handle()
{
    std::uint64_t currtime = now().Int16();
    for (auto it = timerqueue.begin(); it != timerqueue.end(); ) {
        // 已到期
        if (it->first <= currtime) {
            this->flush();
            Timer timer = it->second;
            timer.run();
            if (timer.isPeriodic()) {
                // 周期性定时器
                std::uint64_t endtime = currtime + timer.getInterval() * 1000;
                it = timerqueue.erase(it);
                timerqueue.insert(std::make_pair(endtime, timer));
            } else {
                // 一次性定时器
                it = timerqueue.erase(it);
            }
        } else {
            ++it;
        }
    }
    if (timerqueue.empty()) {
        // 置空
        struct itimerspec its;
        std::memset(&its, 0, sizeof(its));
        if (timerfd_settime(tmfd, 0, &its, nullptr) == -1) {
            ERROR(std::strerror(errno));
        }
    } else {
        // 重新设置
        auto head = timerqueue.begin();
        uint64_t usec = head->first - currtime;
        uint sec = usec / 1000000;
        uint nsec = (usec - sec * 1000000) * 1000;
        Timer timer = head->second;
        uint _interval = timer.getInterval();
        auto interval = ms2SecAndNsec(_interval);
        struct itimerspec its;
        its.it_value.tv_sec = sec;
        its.it_value.tv_nsec = nsec;
        its.it_interval.tv_sec = interval.first;
        its.it_interval.tv_nsec = interval.second;
        if (timerfd_settime(tmfd, 0, &its, nullptr) == -1) {
            ERROR(std::strerror(errno));
        }
    }
}

void TimerQueue::flush()
{
    std::uint64_t msg;
    if (read(tmfd, &msg, sizeof(std::uint64_t)) == -1) {
        ERROR(std::strerror(errno));
    }
}
