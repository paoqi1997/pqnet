#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <sys/epoll.h>
#include <unistd.h>

#include <pqnet/core/timerqueue.h>

static int count = 0;

void sighandler(int signum) {
    std::cout << std::endl;
    std::cout << "Bye!" << std::endl;
}

void printCurrTime() {
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
    printCurrTime();
    std::cout << "Start Timing!" << std::endl;
    tq.addTimer(func, const_cast<char*>("Timer!"), 6000);
    auto id = tq.addTimer(func, const_cast<char*>("Ticker!"), 3000, 1000);
    std::signal(SIGINT, sighandler);
    bool running = true;
    struct epoll_event evpool[evs];
    while (running) {
        int cnt = epoll_wait(epfd, evpool, evs, -1);
        if (cnt == -1) {
            running = false;
            if (errno != EINTR) {
                std::cout << std::strerror(errno) << std::endl;
            }
        }
        for (int i = 0; i < cnt; ++i) {
            if (evpool[i].events & EPOLLIN) {
                printCurrTime();
                tq.handle();
                // Ticker: 9, Timer: 1
                if (++count == 10) {
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
