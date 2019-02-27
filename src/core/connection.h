#ifndef PQNET_CORE_CONNECTION_H
#define PQNET_CORE_CONNECTION_H

#include <cstring>

#include <string>

#include <unistd.h>

#include "buffer.h"

namespace pqnet
{

class TcpConnection
{
public:
    TcpConnection(int _connfd) : connfd(_connfd) {}
    int getFd() const { return connfd; }
    // Remote -> Buffer
    ssize_t recv() { return buffer.readFrom(connfd, buffer.writableBytes()); }
    // Buffer -> Remote
    ssize_t send() { return buffer.writeTo(connfd, buffer.readableBytes()); }
    // Local -> Buffer
    void append(const char *msg) { buffer.append(msg, std::strlen(msg)); }
    // Buffer -> Local
    std::string get() { return buffer.get(buffer.readableBytes()); }
private:
    int connfd;
    Buffer buffer;
};

} // namespace pqnet

#endif // PQNET_CORE_CONNECTION_H
