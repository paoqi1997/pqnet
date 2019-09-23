#ifndef PQNET_CORE_CALLBACK_H
#define PQNET_CORE_CALLBACK_H

#include <functional>
#include <memory>

namespace pqnet
{

using  readHandler = std::function<void()>;
using writeHandler = std::function<void()>;

class TcpConnection;
using TcpConnPtr = std::shared_ptr<TcpConnection>;

using        connectCallBack = std::function<void(const TcpConnPtr&)>;
using          closeCallBack = std::function<void(const TcpConnPtr&)>;
using      implCloseCallBack = std::function<void(const TcpConnPtr&)>;
using messageArrivedCallBack = std::function<void(const TcpConnPtr&)>;
using writeCompletedCallBack = std::function<void(const TcpConnPtr&)>;

} // namespace pqnet

#endif // PQNET_CORE_CALLBACK_H
