#ifndef PQNET_CORE_CALLBACK_H
#define PQNET_CORE_CALLBACK_H

#include <functional>
#include <memory>

namespace pqnet
{

class TcpConnection;

using TcpConnPtr = std::shared_ptr<TcpConnection>;
/*
using  readHandler = std::function<void()>;
using writeHandler = std::function<void()>;
using closeHandler = std::function<void()>;

using connectCallBack = std::function<void(const TcpConnPtr&)>;
using closeCallBack = std::function<void(const TcpConnPtr&)>;
using messageCallBack = std::function<void(const TcpConnPtr&, Buffer*)>;
*/
using     channelCallBack = std::function<void()>;
using  connectionCallBack = std::function<void(const TcpConnPtr&)>;

using          inCallBack = std::function<void(const TcpConnPtr&)>;
using     connectCallBack = std::function<void(const TcpConnPtr&)>;
using        readCallBack = std::function<void(const TcpConnPtr&)>;
using     messageCallBack = std::function<void(const TcpConnPtr&)>;
using closeByPeerCallBack = std::function<void(const TcpConnPtr&)>;
using closeBySockCallBack = std::function<void(const TcpConnPtr&)>;

} // namespace pqnet

#endif // PQNET_CORE_CALLBACK_H
