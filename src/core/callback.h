#ifndef PQNET_CORE_CALLBACK_H
#define PQNET_CORE_CALLBACK_H

#include <functional>
#include <memory>

namespace pqnet
{

class TcpConnection;

using TcpConnPtr = std::shared_ptr<TcpConnection>;

using connectCallBack = std::function<void(const TcpConnPtr&)>;
using   closeCallBack = std::function<void(const TcpConnPtr&)>;
using    readCallBack = std::function<void(const TcpConnPtr&)>;
using messageCallBack = std::function<void(const TcpConnPtr&)>;

}

#endif // PQNET_CORE_CALLBACK_H
