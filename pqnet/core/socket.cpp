#include <fcntl.h>
#include <sys/socket.h>

#include "socket.h"

using namespace pqnet;

int pqnet::new_socket()
{
    return socket(AF_INET, SOCK_STREAM, 0);
}

bool pqnet::setNonBlock(int sockfd, bool flag)
{
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        return false;
    } else {
        if (flag) {
            if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
                return false;
            }
        } else {
            if (fcntl(sockfd, F_SETFL, flags & ~O_NONBLOCK) == -1) {
                return false;
            }
        }
    }
    return true;
}

bool pqnet::setReuseAddr(int sockfd, bool flag)
{
    int opt = flag;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        return false;
    }
    return true;
}

bool pqnet::setReusePort(int sockfd, bool flag)
{
    int opt = flag;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
        return false;
    }
    return true;
}
