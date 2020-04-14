#ifndef PQNET_CORE_HTTP_RESPONSE_H
#define PQNET_CORE_HTTP_RESPONSE_H

#include <string>

namespace pqnet::http
{

class HttpResponse
{
public:
    HttpResponse(std::size_t httpcode = 200);
    const std::string& getResponse() const;
private:
    std::string rep;
};

} // namespace pqnet

#endif // PQNET_CORE_HTTP_RESPONSE_H
