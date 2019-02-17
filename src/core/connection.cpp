#include <cstring>

#include <unistd.h>

#include "connection.h"

using namespace pqnet;

TcpConnection::TcpConnection(int _connfd) : connfd(_connfd)
{

}

TcpConnection::~TcpConnection()
{
    
}

void TcpConnection::recv()
{
    buffer.readFrom(connfd, buffer.writableBytes());
}

void TcpConnection::send()
{
    buffer.writeTo(connfd, buffer.readableBytes());
}

void TcpConnection::send(const char *msg)
{
    write(connfd, msg, std::strlen(msg) + 1);
}
