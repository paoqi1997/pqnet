#include <netinet/in.h>

#include "endian.h"

using namespace pqnet;

std::uint16_t pqnet::h2n16(std::uint16_t h16val)
{
    return htons(h16val);
}

std::uint32_t pqnet::h2n32(std::uint32_t h32val)
{
    return htonl(h32val);
}

std::uint16_t pqnet::n2h16(std::uint16_t n16val)
{
    return ntohs(n16val);
}

std::uint32_t pqnet::n2h32(std::uint32_t n32val)
{
    return ntohl(n32val);
}
