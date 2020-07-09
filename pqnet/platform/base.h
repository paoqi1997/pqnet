#ifndef PQNET_PLATFORM_BASE_H
#define PQNET_PLATFORM_BASE_H

#include <cstddef>
#include <string>

namespace pqnet
{

const char* getErrorMsg(int errCode);

std::size_t getNumberOfProcessors();
int makeDir(const std::string& dirname);

} // namespace pqnet

#endif // PQNET_PLATFORM_BASE_H
