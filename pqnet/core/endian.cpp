#include <arpa/inet.h>

#include "endian.h"

using namespace pqnet;

int pqnet::Endian()
{
    union {
        char s[4];
        std::uint32_t n;
    } un {'L', '?', '?', 'B'};

    char c = static_cast<char>(un.n);
    if (c == 'L') {
        return L_ENDIAN;
    } else if (c == 'B') {
        return B_ENDIAN;
    } else {
        return U_ENDIAN;
    }
}

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
