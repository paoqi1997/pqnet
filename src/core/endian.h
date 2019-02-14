#ifndef PQNET_CORE_ENDIAN_H
#define PQNET_CORE_ENDIAN_H

#include <cstdint>

namespace pqnet
{

std::uint16_t h2n16(std::uint16_t h16val);
std::uint32_t h2n32(std::uint32_t h32val);

std::uint16_t n2h16(std::uint16_t n16val);
std::uint32_t n2h32(std::uint32_t n32val);

} // namespace pqnet

#endif // PQNET_CORE_ENDIAN_H
