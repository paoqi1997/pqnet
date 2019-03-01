#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <cstring>

#include <iostream>

#include <sys/epoll.h>
#include <unistd.h>

#include "../../timerqueue.h"

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
    pqnet::TimerQueue queue;
    struct epoll_event poi;
    poi.data.fd = queue.getFd();
    poi.events = EPOLLET | EPOLLIN;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, queue.getFd(), &poi) == -1) {
        std::cout << std::strerror(errno) << std::endl;
    }
    queue.addTimer(func, const_cast<char*>("Timer!"), 6000);
    queue.addTimer(func, const_cast<char*>("Ticker!"), 3000, 1000);
    print_time();
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
                queue.handle();
                print_time();
            }
        }
    }
    if (close(epfd) == -1) {
        std::cout << std::strerror(errno) << std::endl;
    }
    return 0;
}
