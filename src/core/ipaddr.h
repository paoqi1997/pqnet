#ifndef PQNET_CORE_IPADDR_H
#define PQNET_CORE_IPADDR_H

#include <cstdint>

#include <string>

#include <netinet/in.h>

namespace pqnet
{

class Ip4Addr
{
public:
    Ip4Addr(std::uint16_t port);
    Ip4Addr(const char *hostname, std::uint16_t port);
    ~Ip4Addr();
    std::string ip() const;
    std::uint16_t port() const;
    std::uint32_t ipInt() const;
    std::string toString() const;
private:
    struct sockaddr_in addr;
};

} // namespace pqnet

#endif // PQNET_CORE_IPADDR_H
