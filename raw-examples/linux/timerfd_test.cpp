#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <cstring>

#include <iostream>

#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <unistd.h>

static int count = 0;

void sighandler(int signum) {
    std::cout << std::endl;
    std::cout << "Bye!" << std::endl;
}

void printCurrTime() {
    std::system("date +\"%F %T\"");
}

void handle(int fd, const char *prefix) {
    std::uint64_t msg;
    if (read(fd, &msg, sizeof(std::uint64_t)) == -1) {
        std::cout << std::strerror(errno) << std::endl;
    }
    count += msg;
    std::cout << prefix << ": " << count << std::endl;
}

int main()
{
    const int evs = 8;
    int epfd = epoll_create(evs);
    if (epfd == -1) {
        std::cout << std::strerror(errno) << std::endl;
    }
    int tmfd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK);
    if (tmfd == -1) {
        std::cout << std::strerror(errno) << std::endl;
    }
    struct itimerspec its;
    its.it_value.tv_sec = 3;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 1;
    its.it_interval.tv_nsec = 0;
    if (timerfd_settime(tmfd, 0, &its, nullptr) == -1) {
        std::cout << std::strerror(errno) << std::endl;
    }
    printCurrTime();
    std::cout << "Start Timing!" << std::endl;
    struct epoll_event poi;
    poi.data.fd = tmfd;
    poi.events = EPOLLET | EPOLLIN;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, tmfd, &poi) == -1) {
        std::cout << std::strerror(errno) << std::endl;
    }
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
                int fd = evpool[i].data.fd;
                printCurrTime();
                handle(fd, "Timer");
                if (count == 10) {
                    struct itimerspec nxt_its;
                    std::memset(&nxt_its, 0, sizeof(nxt_its));
                    if (timerfd_settime(fd, 0, &nxt_its, nullptr) == -1) {
                        std::cout << std::strerror(errno) << std::endl;
                    }
                    std::cout << "Stop Timing!" << std::endl;
                }
            }
        }
    }
    if (close(epfd) == -1) {
        std::cout << std::strerror(errno) << std::endl;
    }
    if (close(tmfd) == -1) {
        std::cout << std::strerror(errno) << std::endl;
    }
    return 0;
}
