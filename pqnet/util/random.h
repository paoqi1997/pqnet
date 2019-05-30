#ifndef PQNET_UTIL_RANDOM_H
#define PQNET_UTIL_RANDOM_H

#include <cstdint>

namespace pqnet
{

// [left, right)
std::uint32_t Uint32(std::uint32_t left, std::uint32_t right);
// [left, right)
std::uint64_t Uint64(std::uint64_t left, std::uint64_t right);

} // namespace pqnet

#endif // PQNET_UTIL_RANDOM_H
