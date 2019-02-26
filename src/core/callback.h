#ifndef PQNET_CORE_CALLBACK_H
#define PQNET_CORE_CALLBACK_H

#include <functional>
#include <memory>

namespace pqnet
{

class TcpConnection;

using TcpConnPtr = std::shared_ptr<TcpConnection>;

using     connectCallBack = std::function<void(const TcpConnPtr&)>;
using        readCallBack = std::function<void(const TcpConnPtr&)>;
using     messageCallBack = std::function<void(const TcpConnPtr&)>;
using closeByPeerCallBack = std::function<void(const TcpConnPtr&)>;
using closeBySockCallBack = std::function<void(const TcpConnPtr&)>;

class TcpClientConnection;

using TcpCliConnPtr = std::shared_ptr<TcpClientConnection>;

using     connectCliCallBack = std::function<void(const TcpCliConnPtr&)>;
using        readCliCallBack = std::function<void(const TcpCliConnPtr&)>;
using     messageCliCallBack = std::function<void(const TcpCliConnPtr&)>;
using closeByPeerCliCallBack = std::function<void(const TcpCliConnPtr&)>;
using closeBySockCliCallBack = std::function<void(const TcpCliConnPtr&)>;

} // namespace pqnet

#endif // PQNET_CORE_CALLBACK_H
