#ifndef PQNET_CORE_CONNECTION_H
#define PQNET_CORE_CONNECTION_H

#include <string>

#include <unistd.h>

#include "buffer.h"

namespace pqnet
{

class TcpConnection
{
public:
    TcpConnection(int _connfd);
    ~TcpConnection();
    // Remote -> Buffer
    ssize_t recv();
    // Buffer -> Remote
    ssize_t send();
    // Local -> Buffer
    void append(const char *msg);
    // Buffer -> Local
    std::string get();
    int getFd() const { return connfd; }
private:
    int connfd;
    Buffer buffer;
};

} // namespace pqnet

#endif // PQNET_CORE_CONNECTION_H
