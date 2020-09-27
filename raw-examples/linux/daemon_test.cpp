#include <cerrno>
#include <chrono>
#include <csignal>
#include <cstring>
#include <ctime>
#include <iostream>
#include <thread>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using std::cout;
using std::endl;

char buf[32];

int daemon_init();

const char* getCurrTime() {
    std::time_t time = std::time(nullptr);
    std::tm *group = std::localtime(&time);
    if (std::strftime(buf, sizeof(buf), "%F %T", group) != 0) {
        return buf;
    } else {
        return nullptr;
    }
}

int main(int argc, char *argv[])
{
    bool flag = argc != 2 ? false : std::atoi(argv[1]);
    if (flag) {
        cout << "daemon(3)" << endl;
        /**
         * #include <unistd.h>
         * int daemon(int nochdir, int noclose);
         * 
         * 当nochdir为0时，daemon将该进程的工作目录切换至"/"
         * 当noclose为0时，daemon将该进程的stdin、stdout及stderr都重定向至/dev/null
         */
        if (daemon(1, 1) == -1) {
            cout << std::strerror(errno) << endl;
            return 1;
        }
    } else {
        cout << "daemon_init()" << endl;
        if (daemon_init() == -1) {
            cout << std::strerror(errno) << endl;
            return 1;
        }
    }

    // 0666 & ~002 = 0664
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
        std::this_thread::sleep_for(std::chrono::seconds(3));
        cout << getCurrTime() << " Hello daemon!\n";
        // Flush stdout stream buffer so it goes to correct file.
        std::fflush(stdout);
    }

    return 0;
}

int daemon_init() {
    pid_t pid = fork();
    if (pid == -1) {
        return -1;
    } else if (pid) {
        std::printf("Parent process exited.\n");
        _exit(0);
    }

    if (setsid() == -1) {
        return -1;
    }

    std::signal(SIGHUP, SIG_IGN);

    // ~002
    umask(S_IWOTH);

    pid = fork();
    if (pid == -1) {
        return -1;
    } else if (pid) {
        std::printf("Parent process exited.\n");
        _exit(0);
    }

    return 0;
}
