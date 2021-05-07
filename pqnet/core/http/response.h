#ifndef PQNET_CORE_HTTP_RESPONSE_H
#define PQNET_CORE_HTTP_RESPONSE_H

#include <map>
#include <string>

namespace pqnet
{

namespace http
{

const char* getStatusInfo(std::size_t statusCode);

class HttpResponse
{
public:
    HttpResponse(std::size_t statusCode = 200);
    void resetStatusCode(std::size_t statusCode);
    void addHeader(const std::string& key, const std::string& value) {
        headers[key] = value;
    }
    void appendToBody(const std::string& sBody);
    std::string getResponse() const;
private:
    std::string version;
    // Status Code and Reason Phrase
    std::string statusInfo;
    std::map<std::string, std::string> headers;
    std::string body;
};

} // namespace http

} // namespace pqnet

#endif // PQNET_CORE_HTTP_RESPONSE_H
