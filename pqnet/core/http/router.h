#ifndef PQNET_CORE_HTTP_ROUTER_H
#define PQNET_CORE_HTTP_ROUTER_H

#include <functional>
#include <map>
#include <string>

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
    void addHandler(const std::string& uri, const httpHandler& hd) {
        routingTable[uri] = hd;
    }
    void addStatusCodeHandler(std::size_t statusCode, const httpHandler& hd) {
        scTable[statusCode] = hd;
    }
    void serve(const HttpRequest& req, HttpResponse& resp);
private:
    std::map<std::string, httpHandler> routingTable;
    std::map<std::size_t, httpHandler> scTable;
};

} // namespace http

} // namespace pqnet

#endif // PQNET_CORE_HTTP_ROUTER_H
