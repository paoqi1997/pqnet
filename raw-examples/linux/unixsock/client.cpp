#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "util.h"

int main()
{
    int sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (sockfd == -1) {
        error("socket");
    }

    struct sockaddr_un servaddr;
    std::memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    std::strcpy(servaddr.sun_path, UNIXSOCK);

    auto addrptr = reinterpret_cast<struct sockaddr*>(&servaddr);
    if (connect(sockfd, addrptr, sizeof(struct sockaddr_un)) == -1) {
        error("bind");
    }

    char buf[BUFSIZE] = {0};
    for (;;) {
        std::cin >> buf;

        ssize_t n = write(sockfd, buf, std::strlen(buf));
        if (n == -1) {
            error("write");
        }

        n = read(sockfd, buf, sizeof(buf));
        if (n > 0) {
            std::printf("Recv: %s\n", buf);
        } else if (n == 0) {
            std::printf("Connection closed\n");
            break;
        } else {
            error("read");
            break;
        }
    }

    if (close(sockfd) == -1) {
        error("close");
    }

    return 0;
}
