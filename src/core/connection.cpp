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

TcpClientConnection::TcpClientConnection(int _connfd, TcpEchoClient *_cliptr)
    : connfd(_connfd), cliptr(_cliptr)
{

}

ssize_t TcpClientConnection::recv()
{
    return buffer.readFrom(connfd, buffer.writableBytes());
}

ssize_t TcpClientConnection::send()
{
    return buffer.writeTo(connfd, buffer.readableBytes());
}

void TcpClientConnection::append(const char *msg)
{
    buffer.append(msg, std::strlen(msg));
}

std::string TcpClientConnection::get()
{
    return buffer.get(buffer.readableBytes());
}
