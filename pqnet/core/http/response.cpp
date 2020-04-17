#include "../../util/types.h"
#include "response.h"

using namespace pqnet::http;

HttpResponse::HttpResponse(std::size_t httpcode)
{
    // Status-Line
    if (httpcode == 200) {
        rep.append(HTTP_VERSION_1_1).append(" ").append(HTTP_STATUS_CODE_200).append(CRLF);
    }
    // Headers
    rep.append(CRLF);
}

void HttpResponse::writeBody(const std::string& body)
{
    rep.append(body);
}

const std::string& HttpResponse::getResponse() const
{
    return rep;
}
