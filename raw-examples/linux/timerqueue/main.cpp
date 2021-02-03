#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <sys/epoll.h>
#include <unistd.h>

#include "timerqueue.h"

using std::cout;
using std::endl;

void sighandler(int signum) {
    std::cout << std::endl;
    std::cout << "Bye!" << std::endl;
}

void printCurrTime() {
    std::system("date +\"%F %T\"");
}

int main()
{
    int count = 0;
    // Epfd
    int epfd = epoll_create1(EPOLL_CLOEXEC);
    if (epfd == -1) {
        std::cout << std::strerror(errno) << std::endl;
    }

    // TimerQueue
    TimerQueue tq;

    // Register timerfd to epfd
    struct epoll_event poi;
    poi.data.fd = tq.getFd();
    poi.events = EPOLLET | EPOLLIN;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, tq.getFd(), &poi) == -1) {
        std::cout << std::strerror(errno) << std::endl;
    }

    // Add Timer
    TimerId myID;
    auto cb = [&](void *arg){
        printCurrTime();
        std::cout << static_cast<char*>(arg) << std::endl;
        if (++count == 10) {
            tq.delTimer(myID);
        }
    };

    printCurrTime();
    std::cout << "Start Timing!" << std::endl;

    myID = tq.addTimer(cb, const_cast<char*>("Ticker!"), 3000, 1000);

    // Others
    std::signal(SIGINT, sighandler);

    bool running = true;
    const int evs = 8;
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
            if (evpool[i].data.fd == tq.getFd()) {
                tq.handle();
            }
        }
    }

    if (close(epfd) == -1) {
        std::cout << std::strerror(errno) << std::endl;
    }

    return 0;
}
