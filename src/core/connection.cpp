#include <cstring>

#include "connection.h"

using namespace pqnet;

TcpConnection::TcpConnection(int _connfd) : connfd(_connfd)
{

}

TcpConnection::~TcpConnection()
{

}

ssize_t TcpConnection::recv()
{
    return buffer.readFrom(connfd, buffer.writableBytes());
}

ssize_t TcpConnection::send()
{
    return buffer.writeTo(connfd, buffer.readableBytes());
}

ssize_t TcpConnection::send(const char *msg)
{
    return write(connfd, msg, std::strlen(msg));
}
