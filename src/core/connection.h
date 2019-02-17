#ifndef PQNET_CORE_CONNECTION_H
#define PQNET_CORE_CONNECTION_H

#include "buffer.h"

namespace pqnet
{

class TcpConnection
{
public:
    TcpConnection(int _connfd);
    ~TcpConnection();
    void recv();
    void send();
    void send(const char *msg);
private:
    int connfd;
    Buffer buffer;
};

} // namespace pqnet

#endif // PQNET_CORE_CONNECTION_H
