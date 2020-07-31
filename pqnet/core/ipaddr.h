#ifndef PQNET_CORE_IPADDR_H
#define PQNET_CORE_IPADDR_H

#include <cstdint>
#include <string>

#include <netinet/in.h>
#include <sys/socket.h>

namespace pqnet
{

class Ip4Addr
{
public:
    Ip4Addr(std::uint16_t port);
    Ip4Addr(const char *hostname, std::uint16_t port);
    Ip4Addr(struct sockaddr *address);
    std::string ip() const;
    std::uint16_t port() const;
    std::uint32_t ipInt() const;
    std::string toString() const;
    struct sockaddr_in* getPtr() { return &addr; }
private:
    struct sockaddr_in addr;
};

} // namespace pqnet

#endif // PQNET_CORE_IPADDR_H
