#ifndef PQNET_CORE_CONNECTION_H
#define PQNET_CORE_CONNECTION_H

#include <cstring>
#include <memory>
#include <string>

#include "buffer.h"
#include "callback.h"
#include "eventloop.h"
#include "trigger.h"

namespace pqnet
{

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(EventLoop *_looper, int fd);
    int getFd() const { return tg->getFd(); }
    EventLoop* getEventLoop() { return looper; }
    Buffer* getInputBuffer() { return &inputBuffer; }
    Buffer* getOutputBuffer() { return &outputBuffer; }
    void connectEstablished();
    void connectDestroyed();
    bool isConnected() const { return connected; }
    void send(const char *data, std::size_t len);
    void setConnectCallBack(const connectCallBack& cb) { conncb = cb; }
    void setCloseCallBack(const closeCallBack& cb) { closecb = cb; }
    void setRemoveConnCallBack(const removeConnCallBack& cb) { rmcb = cb; }
    void setMessageArrivedCallBack(const messageArrivedCallBack& cb) { macb = cb; }
    void setWriteCompletedCallBack(const writeCompletedCallBack& cb) { wccb = cb; }
private:
    void handleRead();
    void handleWrite();
    void handleClose();
    connectCallBack conncb;
    closeCallBack closecb;
    removeConnCallBack rmcb;
    messageArrivedCallBack macb;
    writeCompletedCallBack wccb;
    Buffer inputBuffer;
    Buffer outputBuffer;
    bool connected;
    EventLoop *looper;
    std::unique_ptr<Trigger> tg;
};

} // namespace pqnet

#endif // PQNET_CORE_CONNECTION_H
