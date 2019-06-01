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
    void setReadCallBack(const readCallBack& cb) { readcb = cb; }
    void setMessageCallBack(const messageCallBack& cb) { msgcb = cb; }
    void setCloseByPeerCallBack(const closeByPeerCallBack& cb) { cpcb = cb; }
    void setCloseBySockCallBack(const closeBySockCallBack& cb) { cscb = cb; }
private:
    connectCallBack conncb;
    readCallBack readcb;
    messageCallBack msgcb;
    closeByPeerCallBack cpcb;
    closeBySockCallBack cscb;
    std::size_t ln;
public:
    std::vector<std::unique_ptr<Looper>> loopers;
};

} // namespace pqnet

#endif // PQNET_CORE_LOOPER_POOL_H
