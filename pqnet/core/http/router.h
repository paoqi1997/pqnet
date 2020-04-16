#ifndef PQNET_CORE_HTTP_ROUTER_H
#define PQNET_CORE_HTTP_ROUTER_H

#include <functional>
#include <map>
#include <string>
#include <utility>

#include "request.h"
#include "response.h"

namespace pqnet
{

namespace http
{

using httpHandler = std::function<void(const HttpRequest&, HttpResponse&)>;

class HttpRouter
{
public:
    HttpRouter();
    const httpHandler& getHandler(const std::string& uri);
    void route(const std::string& method, const std::string& uri, const httpHandler& hd);
private:
    // kv: <uri, <method, httpHandler>>
    std::map<std::string, std::pair<std::string, httpHandler>> routingTable;
};

} // namespace http

} // namespace pqnet

#endif // PQNET_CORE_HTTP_ROUTER_H
