#ifndef PQNET_CORE_CONNECTION_H
#define PQNET_CORE_CONNECTION_H

#include <cstring>
#include <memory>
#include <string>

#include "buffer.h"
#include "callback.h"
#include "channel.h"

namespace pqnet
{

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(int epfd, int _connfd);
    int getFd() const { return connfd; }
    // Remote -> Buffer
    ssize_t recv() { return buffer.readFrom(connfd, buffer.writableBytes()); }
    // Buffer -> Remote
    ssize_t send() { return buffer.writeTo(connfd, buffer.readableBytes()); }
    // Local -> Buffer
    void append(const char *msg) { buffer.append(msg, std::strlen(msg)); }
    // Buffer -> Local
    std::string get() { return inputBuffer.get(inputBuffer.readableBytes()); }
    void send(const char *data, std::size_t len);
    void setConnectCallBack(const connectionCallBack& cb) { conncb = cb; }
    void setCloseCallBack(const connectionCallBack& cb) { closecb = cb; }
    void setMessageCallBack(const connectionCallBack& cb) { msgcb = cb; }
    void setWriteCompletedCallBack(const connectionCallBack& cb) { wccb = cb; }
private:
    void handleRead();
    void handleWrite();
    void handleClose();
    int connfd;
    Buffer buffer;
    Buffer inputBuffer;
    Buffer outputBuffer;
    std::unique_ptr<Channel> ch;
    connectionCallBack conncb;
    connectionCallBack closecb;
    connectionCallBack msgcb;
    connectionCallBack wccb;
};

} // namespace pqnet

#endif // PQNET_CORE_CONNECTION_H
