#include <cstdlib>
#include <csignal>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "util.h"

int listenfd = -1;

void onSignal(int signum)
{
    if (unlink(UNIXSOCK) == -1) {
        error("unlink");
    }

    if (close(listenfd) == -1) {
        error("close");
    }

    std::cout << std::endl;
    std::exit(EXIT_SUCCESS);
}

int main()
{
    listenfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (listenfd == -1) {
        error("socket");
    }

    if (std::signal(SIGINT, onSignal) == SIG_ERR) {
        error("signal");
    }

    struct sockaddr_un servaddr;
    std::memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    std::strcpy(servaddr.sun_path, UNIXSOCK);

    auto addrptr = reinterpret_cast<struct sockaddr*>(&servaddr);
    if (bind(listenfd, addrptr, sizeof(struct sockaddr_un)) == -1) {
        error("bind");
    }

    if (listen(listenfd, BACKLOG) == -1) {
        error("listen");
    }

    int connfd = -1;
    struct sockaddr_un cliaddr;
    addrptr = reinterpret_cast<struct sockaddr*>(&cliaddr);
    socklen_t clilen = sizeof(struct sockaddr_un);

    char buf[BUFSIZE] = {0};
    for (;;) {
        if (connfd == -1) {
            connfd = accept(listenfd, addrptr, &clilen);
            if (connfd == -1) {
                error("accept");
                break;
            }
            std::printf("Connection %d coming...\n", connfd);
        }

        ssize_t n = read(connfd, buf, sizeof(buf));
        if (n > 0) {
            std::printf("Recv: %s\n", buf);
            n = write(connfd, buf, n);
            if (n == -1) {
                error("write");
                break;
            }
        } else if (n == 0) {
            std::printf("Connection %d closing...\n", connfd);
            if (close(connfd) == -1) {
                error("close");
                break;
            }
            connfd = -1;
            continue;
        } else {
            error("read");
            break;
        }
    }

    return 0;
}
