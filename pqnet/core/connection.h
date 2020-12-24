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
    enum Status {
        DISCONNECTED,
        CONNECTING,
        CONNECTED,
        DISCONNECTING
    };
    TcpConnection(EventLoop *_looper, int fd);
    int getFd() const { return tg->getFd(); }
    EventLoop* getEventLoop() { return looper; }
    Buffer* getInputBuffer() { return &inputBuffer; }
    Buffer* getOutputBuffer() { return &outputBuffer; }
    void connectEstablished();
    void connectDestroyed();
    Status getStatus() const { return status; }
    void send(const char *data, std::size_t len);
    void shutdown();
    void setConnectCallBack(const connectCallBack& cb) { conncb = cb; }
    void setCloseCallBack(const closeCallBack& cb) { closecb = cb; }
    void setRemoveConnCallBack(const removeConnCallBack& cb) { rmcb = cb; }
    void setMessageArrivedCallBack(const messageArrivedCallBack& cb) { macb = cb; }
    void setWriteCompletedCallBack(const writeCompletedCallBack& cb) { wccb = cb; }
private:
    void shutdownInLoop();
    void handleRead();
    void handleWrite();
    void handleClose();
    void handleError();
    connectCallBack conncb;
    closeCallBack closecb;
    removeConnCallBack rmcb;
    messageArrivedCallBack macb;
    writeCompletedCallBack wccb;
    Buffer inputBuffer;
    Buffer outputBuffer;
    Status status;
    EventLoop *looper;
    std::unique_ptr<Trigger> tg;
};

} // namespace pqnet

#endif // PQNET_CORE_CONNECTION_H
