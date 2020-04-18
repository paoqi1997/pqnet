#ifndef PQNET_CORE_HTTP_REQUEST_H
#define PQNET_CORE_HTTP_REQUEST_H

#include <map>
#include <string>

namespace pqnet
{

namespace http
{

class HttpRequest
{
public:
    enum Part {
        REQUEST_LINE,
        HEADERS,
        BODY
    };
    HttpRequest(const std::string& sReq);
    void addHeader(const std::string& key, const std::string& value) {
        headers[key] = value;
    }
    std::string getRequest() const;
private:
    void parseRequestLine(const std::string& reqLine);
private:
    std::string method;
    std::string uri;
    std::string version;
    std::map<std::string, std::string> headers;
    std::string body;
};

} // namespace http

} // namespace pqnet

#endif // PQNET_CORE_HTTP_REQUEST_H
