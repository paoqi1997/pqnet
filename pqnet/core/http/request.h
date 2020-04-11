#ifndef PQNET_CORE_HTTP_REQUEST_H
#define PQNET_CORE_HTTP_REQUEST_H

#include <map>
#include <string>

namespace pqnet
{

class HttpRequest
{
public:
    HttpRequest(const std::string& req);
private:
    void parseRequestLine(const std::string& reqLine);
private:
    std::string method;
    std::string uri;
    std::string version;
    std::map<std::string, std::string> headers;
    std::string body;
};

} // namespace pqnet

#endif // PQNET_CORE_HTTP_REQUEST_H
