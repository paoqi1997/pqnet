#ifndef PQNET_CORE_CONNECTION_H
#define PQNET_CORE_CONNECTION_H

#include "buffer.h"

namespace pqnet
{

class TcpConnection
{
public:
    TcpConnection();
    ~TcpConnection();
    void onRead();
    void onWrite();
    void onClose();
private:
    bool connected;
    int connfd;
    Buffer buffer;
};

} // namespace pqnet

#endif // PQNET_CORE_CONNECTION_H
