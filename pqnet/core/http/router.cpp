#include "router.h"

using namespace pqnet::http;

HttpRouter::HttpRouter()
{

}

void HttpRouter::serve(const HttpRequest& req, HttpResponse& resp)
{
    std::string uri = req.getUri();
    if (routingTable.find(uri) != routingTable.end()) {
        routingTable[uri](req, resp);
    }
}
