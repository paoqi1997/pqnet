#include "router.h"

using namespace pqnet::http;

HttpRouter::HttpRouter()
{
    scTable[404] = [](const HttpRequest& req, HttpResponse& resp) {
        std::string body;
        body.append(
            "<!DOCTYPE html>\n"
            "<html>\n"
            "<head>\n"
            "    <title>404 Not Found</title>\n"
            "</head>\n"
            "<body>\n"
            "    <center><h1>404 Not Found</h1></center>\n"
            "    <hr>\n"
            "    <center>httpserver/0.1.0</center>\n"
            "</body>\n"
            "</html>\n"
        );
        resp.resetStatusCode(404);
        resp.addHeader("Content-Length", std::to_string(body.size()));
        resp.addHeader("Content-Type", "text/html; charset=utf-8");
        resp.appendToBody(body);
    };
}

void HttpRouter::serve(const HttpRequest& req, HttpResponse& resp)
{
    std::string uri = req.getUri();

    if (routingTable.find(uri) != routingTable.end()) {
        routingTable[uri](req, resp);
    } else {
        scTable[404](req, resp);
    }
}
