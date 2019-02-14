#include <cstring>

#include <vector>

#include <arpa/inet.h>
#include <netdb.h>

#include "endian.h"
#include "ipaddr.h"

using namespace pqnet;

Ip4Addr::Ip4Addr(std::uint16_t port)
{
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = h2n16(port);
    addr.sin_addr.s_addr = h2n32(INADDR_ANY);
}

Ip4Addr::Ip4Addr(const char *hostname, std::uint16_t port)
{
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = h2n16(port);
    struct hostent *he = gethostbyname(hostname);
    if (he != nullptr && he->h_addrtype == AF_INET) {
        addr.sin_addr = *reinterpret_cast<struct in_addr*>(he->h_addr);
    } else {
        addr.sin_addr.s_addr = h2n32(INADDR_ANY);
    }
}

Ip4Addr::~Ip4Addr()
{

}

std::string Ip4Addr::ip() const
{
    std::vector<char> buf(INET_ADDRSTRLEN);
    auto addrptr = static_cast<const void*>(&addr.sin_addr);
    return std::string(inet_ntop(AF_INET, addrptr, buf.data(), buf.size()));
}

std::uint16_t Ip4Addr::port() const
{
    return n2h16(addr.sin_port);
}

std::uint32_t Ip4Addr::ipInt() const
{
    return n2h32(addr.sin_addr.s_addr);
}

std::string Ip4Addr::toString() const
{
    return this->ip() + ':' + std::to_string(this->port());
}
