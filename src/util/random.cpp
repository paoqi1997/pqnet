#include <random>

#include "random.h"
#include "timestamp.h"

using namespace pqnet;

namespace {

// Engine
std::default_random_engine engine(now().Int16());
// Distribution
std::uniform_int_distribution<std::uint32_t> dist32;
std::uniform_int_distribution<std::uint64_t> dist64;

}

std::uint32_t pqnet::Uint32(std::uint32_t left, std::uint32_t right)
{
    if (left < right) {
        return dist32(engine) % (right - left) + left;
    } else if (left == right) {
        return left;
    } else {
        return dist32(engine) % (left - right) + right;
    }
}

std::uint64_t pqnet::Uint64(std::uint64_t left, std::uint64_t right)
{
    if (left < right) {
        return dist64(engine) % (right - left) + left;
    } else if (left == right) {
        return left;
    } else {
        return dist64(engine) % (left - right) + right;
    }
}
