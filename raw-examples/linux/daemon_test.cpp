#include <cerrno>
#include <chrono>
#include <csignal>
#include <cstring>
#include <iostream>
#include <thread>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
    pid_t pid = fork();
    if (pid == -1) {
        cout << std::strerror(errno) << endl;
        return 1;
    } else if (pid) {
        std::printf("Parent process exited.\n");
        _exit(0);
    }

    if (setsid() == -1) {
        cout << std::strerror(errno) << endl;
        return 1;
    }

    std::signal(SIGHUP, SIG_IGN);

    // ~022
    umask(S_IWGRP | S_IWOTH);

    pid = fork();
    if (pid == -1) {
        cout << std::strerror(errno) << endl;
        return 1;
    } else if (pid) {
        std::printf("Parent process exited.\n");
        _exit(0);
    }

    // 0666 & ~022 = 0644
    int fd = open("./daemon.log", O_CREAT | O_WRONLY, 0666);
    if (fd == -1) {
        cout << std::strerror(errno) << endl;
        return 1;
    } else {
        if (dup2(fd, STDIN_FILENO) == -1) {
            cout << std::strerror(errno) << endl;
            return 1;
        }
        if (dup2(fd, STDOUT_FILENO) == -1) {
            cout << std::strerror(errno) << endl;
            return 1;
        }
        if (dup2(fd, STDERR_FILENO) == -1) {
            cout << std::strerror(errno) << endl;
            return 1;
        }
        if (fd > STDERR_FILENO) {
            if (close(fd) == -1) {
                cout << std::strerror(errno) << endl;
                return 1;
            }
        }
    }

    for (;;) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        cout << "Hello daemon!\n";
        // Flush stdout stream buffer so it goes to correct file.
        std::fflush(stdout);
    }

    return 0;
}
