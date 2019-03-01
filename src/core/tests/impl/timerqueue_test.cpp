#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <cstring>

#include <iostream>

#include <sys/epoll.h>
#include <unistd.h>

#include "../../timerqueue.h"

static int count = 0;

void sighandler(int signum) {
    std::cout << std::endl;
    std::cout << "Bye!" << std::endl;
}

void print_time() {
    std::system("date +\"%F %T\"");
}

void func(void *arg) {
    std::cout << static_cast<char*>(arg) << std::endl;
}

int main()
{
    const int evs = 8;
    int epfd = epoll_create(evs);
    if (epfd == -1) {
        std::cout << std::strerror(errno) << std::endl;
    }
    pqnet::TimerQueue tq;
    struct epoll_event poi;
    poi.data.fd = tq.getFd();
    poi.events = EPOLLET | EPOLLIN;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, tq.getFd(), &poi) == -1) {
        std::cout << std::strerror(errno) << std::endl;
    }
    print_time();
    std::cout << "Start Timing!" << std::endl;
    tq.addTimer(func, const_cast<char*>("Timer!"), 6000);
    auto id = tq.addTimer(func, const_cast<char*>("Ticker!"), 3000, 1000);
    std::signal(SIGINT, sighandler);
    bool running = true;
    struct epoll_event evpool[evs];
    while (running) {
        int cnt = epoll_wait(epfd, evpool, evs, -1);
        if (cnt == -1) {
            if (errno == EINTR) {
                running = false;
            } else {
                std::cout << std::strerror(errno) << std::endl;
                break;
            }
        }
        for (int i = 0; i < cnt; ++i) {
            if (evpool[i].events & EPOLLIN) {
                print_time();
                // Ticker -> Ticker -> Ticker -> Timer -> Ticker -> ...
                // 经历 Ticker -> Timer 后
                // 下一次 Ticker 的时间戳将大于 currtime
                // Ticker 共执行了 9 次
                tq.handle();
                ++count;
                if (count == 10) {
                    tq.delTimer(id);
                }
            }
        }
    }
    if (close(epfd) == -1) {
        std::cout << std::strerror(errno) << std::endl;
    }
    return 0;
}
