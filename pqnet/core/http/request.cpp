#include <cstring>

#include "../../util/logger.h"
#include "request.h"

using namespace pqnet;

const char *HttpRequest::CRLF = "\r\n";

HttpRequest::HttpRequest(const std::string& req)
{
    int flag = 0;
    for (std::size_t begin = 0, end = 0; end < req.length(); ) {
        if (req.compare(end, 2, CRLF) == 0) {
            if (flag == 0) {
                line = req.substr(begin, end - begin);
                parseRequestLine();
                flag = 1;
            } else if (flag == 1) {
                std::size_t pos = req.find(':', begin);
                std::string key = req.substr(begin, pos - begin);
                std::string value = req.substr(pos + 2, end - (pos + 2));
                headers[key] = value;
            } else if (flag == 2) {
                body = req.substr(begin, end - begin);
            }
            if (req.compare(end + 2, 2, CRLF) == 0) {
                begin = end = end + 4;
                if (flag == 1) {
                    flag = 2;
                }
            } else {
                begin = end = end + 2;
            }
        } else {
            ++end;
        }
    }
}

void HttpRequest::parseRequestLine()
{
    std::size_t pos1 = line.find(' ');
    std::size_t pos2 = line.find(' ', pos1 + 1);
    method = line.substr(0, pos1);
    uri = line.substr(pos1 + 1, pos2 - (pos1 + 1));
    version = line.substr(pos2 + 1);
    INFO("%s", line.c_str());
    INFO("%s %s %s", method.c_str(), uri.c_str(), version.c_str());
}
