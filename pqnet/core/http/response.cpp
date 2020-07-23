#include "../../util/defines.h"
#include "response.h"

using namespace pqnet::http;

const char* pqnet::http::getStatusInfo(std::size_t statusCode)
{
    switch (statusCode) {
    case 200:
        return HTTP_STATUS_CODE_200;
    case 404:
        return HTTP_STATUS_CODE_404;
    default:
        return nullptr;
    }
}

HttpResponse::HttpResponse(std::size_t statusCode) : version(HTTP_VERSION_1_1)
{
    statusInfo = getStatusInfo(statusCode);
}

void HttpResponse::appendToBody(const std::string& sBody)
{
    body.append(sBody);
}

std::string HttpResponse::getResponse() const
{
    std::string rep;
    // Status-Line
    rep.append(version).append(" ").append(statusInfo).append(CRLF);
    // Headers
    for (auto& header : headers) {
        rep.append(header.first).append(": ").append(header.second).append(CRLF);
    }
    rep.append(CRLF);
    // Body
    rep.append(body);
    return rep;
}
