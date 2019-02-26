#include <cstring>

#include "connection.h"

using namespace pqnet;

TcpConnection::TcpConnection(int _connfd) : connfd(_connfd)
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

void TcpConnection::append(const char *msg)
{
    buffer.append(msg, std::strlen(msg));
}

std::string TcpConnection::get()
{
    return buffer.get(buffer.readableBytes());
}
