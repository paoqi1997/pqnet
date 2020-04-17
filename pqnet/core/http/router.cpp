#include "../../util/types.h"
#include "router.h"

using namespace pqnet::http;

HttpRouter::HttpRouter()
{
    routingTable["/"] = std::make_pair("GET", [](const HttpRequest& req, HttpResponse& rep){
        // Body
        std::string body;
        body.append(
            "<!DOCTYPE html>\n"
            "<html>\n"
            "<head>\n"
            "    <title>index.html</title>\n"
            "</head>\n"
            "<body>\n"
            "    <a>TEST</a>\n"
            "</body>\n"
            "</html>"
        );
        rep.writeBody(body);
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
