#include <fcntl.h>
#include <netinet/in.h>  // for IPPROTO_TCP
#include <netinet/tcp.h> // for TCP_NODELAY
#include <sys/socket.h>

#include "../util/defines.h"
#include "socket.h"

using namespace pqnet;

int pqnet::new_socket()
{
    return socket(AF_INET, SOCK_STREAM, 0);
}

int pqnet::shutdownWrite(int sockfd)
{
    return shutdown(sockfd, SHUT_WR);
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

int pqnet::isNoDelay(int sockfd)
{
    int opt;
    socklen_t optlen = sizeof(opt);
    if (getsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &opt, &optlen) == -1) {
        return UNKNOWN;
    }
    return opt;
}

int pqnet::isKeepAlive(int sockfd)
{
    int opt;
    socklen_t optlen = sizeof(opt);
    if (getsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &opt, &optlen) == -1) {
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

bool pqnet::setNoDelay(int sockfd, bool flag)
{
    int opt = flag;
    if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt)) == -1) {
        return false;
    }
    return true;
}

bool pqnet::setKeepAlive(int sockfd, bool flag)
{
    int opt = flag;
    if (setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt)) == -1) {
        return false;
    }
    return true;
}
