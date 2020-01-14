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

    char buf[32];
    if (getcwd(buf, sizeof(buf)) == nullptr) {
        cout << std::strerror(errno) << endl;
        return 1;
    } else {
        std::printf("CurrWorkDir: %s\n", buf);
    }

    if (chdir("/") == -1) {
        cout << std::strerror(errno) << endl;
        return 1;
    }

    if (getcwd(buf, sizeof(buf)) == nullptr) {
        cout << std::strerror(errno) << endl;
        return 1;
    } else {
        std::printf("CurrWorkDir: %s\n", buf);
    }

    std::signal(SIGHUP, SIG_IGN);

    umask(0);

    pid = fork();
    if (pid == -1) {
        cout << std::strerror(errno) << endl;
        return 1;
    } else if (pid) {
        std::printf("Parent process exited.\n");
        _exit(0);
    }

    int fd = open("/dev/null", O_RDWR);
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
    }

    return 0;
}
