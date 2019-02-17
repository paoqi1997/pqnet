#ifndef PQNET_CORE_CONNECTION_H
#define PQNET_CORE_CONNECTION_H

#include <unistd.h>

#include "buffer.h"

namespace pqnet
{

class TcpConnection
{
public:
    TcpConnection(int _connfd);
    ~TcpConnection();
    ssize_t recv();
    ssize_t send();
    ssize_t send(const char *msg);
private:
    int connfd;
    Buffer buffer;
};

} // namespace pqnet

#endif // PQNET_CORE_CONNECTION_H
