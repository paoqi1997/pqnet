#include "router.h"

using namespace pqnet::http;

HttpRouter::HttpRouter()
{

}

void HttpRouter::serve(const HttpRequest& req, HttpResponse& rep)
{
    std::string uri = req.getUri();
    if (routingTable.find(uri) != routingTable.end()) {
        routingTable[uri](req, rep);
    }
}
