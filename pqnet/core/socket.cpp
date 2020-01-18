#include <fcntl.h>
#include <sys/socket.h>

#include "socket.h"
#include "../util/types.h"

using namespace pqnet;

int pqnet::new_socket()
{
    return socket(AF_INET, SOCK_STREAM, 0);
}

int pqnet::isNonBlock(int sockfd)
{
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        return UNKNOWN;
    } else {
        if (flags & O_NONBLOCK) {
            return TRUE;
        } else {
            return FALSE;
        }
    }
}

int pqnet::isReuseAddr(int sockfd)
{
    int opt;
    socklen_t optlen = sizeof(opt);
    if (getsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, &optlen) == -1) {
        return UNKNOWN;
    }
    return opt;
}

int pqnet::isReusePort(int sockfd)
{
    int opt;
    socklen_t optlen = sizeof(opt);
    if (getsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, &optlen) == -1) {
        return UNKNOWN;
    }
    return opt;
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
