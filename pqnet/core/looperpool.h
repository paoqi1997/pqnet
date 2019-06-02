#ifndef PQNET_CORE_LOOPER_POOL_H
#define PQNET_CORE_LOOPER_POOL_H

#include <memory>
#include <vector>

#include "callback.h"
#include "looper.h"

namespace pqnet
{

class LooperPool
{
public:
    LooperPool(std::size_t looperNumber);
    LooperPool(std::size_t looperNumber, pn_thread_func_t func);
    ~LooperPool();
    void run();
    void setConnectCallBack(const connectCallBack& cb) { conncb = cb; }
    void setCloseCallBack(const closeCallBack& cb) { closecb = cb; }
    void setMessageArrivedCallBack(const messageArrivedCallBack& cb) { macb = cb; }
    void setWriteCompletedCallBack(const writeCompletedCallBack& cb) { wccb = cb; }
private:
    connectCallBack conncb;
    closeCallBack closecb;
    messageArrivedCallBack macb;
    writeCompletedCallBack wccb;
    std::size_t ln;
public:
    std::vector<std::unique_ptr<Looper>> loopers;
};

} // namespace pqnet

#endif // PQNET_CORE_LOOPER_POOL_H
