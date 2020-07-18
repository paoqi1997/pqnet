#ifndef PQNET_PLATFORM_BASE_H
#define PQNET_PLATFORM_BASE_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <thread>

namespace pqnet
{

const char* getErrorMsg(int errCode);

std::size_t getNumberOfProcessors();
int makeDir(const std::string& dirname);

std::uint64_t tid2u64(std::thread::id tid);

} // namespace pqnet

#endif // PQNET_PLATFORM_BASE_H
