#ifndef PQNET_CORE_CONNECTION_H
#define PQNET_CORE_CONNECTION_H

#include <cstring>
#include <memory>
#include <string>

#include "buffer.h"
#include "callback.h"
#include "trigger.h"

namespace pqnet
{

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(int epfd, int _connfd);
    int getFd() const { return tg->getFd(); }
    Buffer* getInputBuffer() { return &inputBuffer; }
    Buffer* getOutputBuffer() { return &outputBuffer; }
    void connectEstablished();
    void connectDestroyed();
    bool isConnected() const { return connected; }
    void send(const char *data, std::size_t len);
    void setConnectCallBack(const connectCallBack& cb) { conncb = cb; }
    void setCloseCallBack(const closeCallBack& cb) { closecb = cb; }
    void setImplCloseCallBack(const implCloseCallBack& cb) { iccb = cb; }
    void setMessageArrivedCallBack(const messageArrivedCallBack& cb) { macb = cb; }
    void setWriteCompletedCallBack(const writeCompletedCallBack& cb) { wccb = cb; }
private:
    void handleRead();
    void handleWrite();
    connectCallBack conncb;
    closeCallBack closecb;
    implCloseCallBack iccb;
    messageArrivedCallBack macb;
    writeCompletedCallBack wccb;
    Buffer inputBuffer;
    Buffer outputBuffer;
    bool connected;
    std::unique_ptr<Trigger> tg;
};

} // namespace pqnet

#endif // PQNET_CORE_CONNECTION_H
