#include <cstdint>
#include <iostream>

#include <endian.h>

int main()
{
    std::uint16_t h16val = 8088;
    std::uint32_t h32val = 12488;

    std::uint16_t n16val = pqnet::h2n16(h16val);
    std::uint32_t n32val = pqnet::h2n32(h32val);

    std::uint16_t _h16val = pqnet::n2h16(n16val);
    std::uint32_t _h32val = pqnet::n2h32(n32val);

    std::cout << h16val << ' ' << n16val << ' ' << _h16val << std::endl;
    std::cout << h32val << ' ' << n32val << ' ' << _h32val << std::endl;

    return 0;
}
