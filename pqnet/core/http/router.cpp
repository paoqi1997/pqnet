#include "router.h"

using namespace pqnet::http;

HttpRouter::HttpRouter()
{
    routingTable["/"] = std::make_pair("GET", [](const HttpRequest& req, HttpResponse& rep){
        rep.writeBody("<html><title>index.html</title></html>");
    });
}

const httpHandler& HttpRouter::getHandler(const std::string& uri)
{
    if (routingTable.find(uri) != routingTable.end()) {
        return routingTable[uri].second;
    } else {
        return routingTable["/"].second;
    }
}
