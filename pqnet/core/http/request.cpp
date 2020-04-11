#include <cstring>

#include "../../util/logger.h"
#include "../../util/types.h"
#include "request.h"

using namespace pqnet;

HttpRequest::HttpRequest(const std::string& req)
{
    int flag = 0;
    for (std::size_t begin = 0, end = 0; end < req.length(); ) {
        // ...\r\n
        if (req.compare(end, 2, CRLF) == 0) {
            if (flag == 0) {
                // Request-Line
                std::string reqLine = req.substr(begin, end - begin);
                parseRequestLine(reqLine);
                flag = 1;
            } else if (flag == 1) {
                // Headers
                std::size_t pos = req.find(':', begin);
                std::string key = req.substr(begin, pos - begin);
                std::string value = req.substr(pos + 2, end - (pos + 2));
                headers[key] = value;
            } else if (flag == 2) {
                // Body
                body = req.substr(begin, end - begin);
            }
            // ...\r\n
            // \r\n
            if (req.compare(end + 2, 2, CRLF) == 0) {
                begin = end = end + 4;
                if (flag == 1) {
                    flag = 2;
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
