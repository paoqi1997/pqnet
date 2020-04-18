#include "../../util/logger.h"
#include "../../util/types.h"
#include "request.h"

using namespace pqnet::http;

HttpRequest::HttpRequest(const std::string& sReq)
{
    int part = 1;
    for (std::size_t begin = 0, end = 0; end < sReq.length(); ) {
        // ...\r\n
        if (sReq.compare(end, 2, CRLF) == 0) {
            if (part == 1) {
                // Request-Line
                std::string reqLine = sReq.substr(begin, end - begin);
                parseRequestLine(reqLine);
                part = 2;
            } else if (part == 2) {
                // Headers
                std::size_t pos = sReq.find(':', begin);
                std::string key = sReq.substr(begin, pos - begin);
                std::string value = sReq.substr(pos + 2, end - (pos + 2));
                headers[key] = value;
            } else if (part == 3) {
                // Body
                body = sReq.substr(begin, end - begin);
            }
            // ...\r\n
            // \r\n
            if (sReq.compare(end + 2, 2, CRLF) == 0) {
                begin = end = end + 4;
                if (part == 2) {
                    part = 3;
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

void HttpRequest::parseRequestLine(const std::string& reqLine)
{
    std::size_t pos1 = reqLine.find(' ');
    std::size_t pos2 = reqLine.find(' ', pos1 + 1);
    method = reqLine.substr(0, pos1);
    uri = reqLine.substr(pos1 + 1, pos2 - (pos1 + 1));
    version = reqLine.substr(pos2 + 1);
}
