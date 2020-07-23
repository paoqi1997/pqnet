#include "../../util/defines.h"
#include "request.h"

using namespace pqnet::http;

HttpRequest::HttpRequest(const std::string& sReq)
{
    Part part = REQUEST_LINE;
    for (std::size_t begin = 0, end = 0; end < sReq.length(); ) {
        // ...\r\n
        if (sReq.compare(end, 2, CRLF) == 0) {
            if (part == REQUEST_LINE) {
                // Request-Line
                std::string reqLine = sReq.substr(begin, end - begin);
                parseRequestLine(reqLine);
                part = HEADERS;
            } else if (part == HEADERS) {
                // Headers
                std::size_t pos = sReq.find(':', begin);
                std::string key = sReq.substr(begin, pos - begin);
                std::string value = sReq.substr(pos + 2, end - (pos + 2));
                headers[key] = value;
            } else if (part == BODY) {
                // Body
                body = sReq.substr(begin, end - begin);
            }
            // ...\r\n
            // \r\n
            if (sReq.compare(end + 2, 2, CRLF) == 0) {
                begin = end = end + 4;
                if (part == HEADERS) {
                    part = BODY;
                }
            }
            // ...\r\n
            // ...\r\n
            else {
                begin = end = end + 2;
            }
        } else {
            ++end;
        }
    }
}

std::string HttpRequest::getRequest() const
{
    std::string req;
    // Request-Line
    req.append(method).append(" ").append(uri).append(" ").append(version).append(CRLF);
    // Headers
    for (auto& header : headers) {
        req.append(header.first).append(": ").append(header.second).append(CRLF);
    }
    req.append(CRLF);
    // Body
    req.append(body);
    return req;
}

void HttpRequest::parseRequestLine(const std::string& reqLine)
{
    std::size_t pos1 = reqLine.find(' ');
    std::size_t pos2 = reqLine.find(' ', pos1 + 1);
    method = reqLine.substr(0, pos1);
    uri = reqLine.substr(pos1 + 1, pos2 - (pos1 + 1));
    version = reqLine.substr(pos2 + 1);
}
