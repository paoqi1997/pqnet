#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/stat.h> // for mkdir()
#include <unistd.h>   // for access()
#endif

#include "../util/types.h"
#include "base.h"

using namespace pqnet;

namespace {
std::size_t nProcessors = 0;
} // unnamed namespace

const char* pqnet::getErrorMsg(int errCode)
{
    if (errCode == errno) {
        return std::strerror(errno);
    }
    switch (errCode) {
    case 0:
        return "ok.";
#ifdef WIN32
    case ERROR_ALREADY_EXISTS:
        return "The specified directory already exists.";
    case ERROR_PATH_NOT_FOUND:
        return "One or more intermediate directories do not exist.";
#endif
    default:
        return "unknown error.";
    }
}

std::size_t pqnet::getNumberOfProcessors()
{
#ifdef WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
#else
    if (nProcessors == 0) {
        const char *sFileName = "/proc/cpuinfo";
        std::ifstream is(sFileName);
        if (!is.is_open()) {
            nProcessors = 1;
        } else {
            std::string line;
            while (!is.eof()) {
                std::getline(is, line);
                if (line.find("processor") != std::string::npos) {
                    ++nProcessors;
                }
            }
        }
        if (is.is_open()) {
            is.close();
        }
    }
    return nProcessors;
#endif
}

int pqnet::makeDir(const std::string& dirname)
{
#ifdef WIN32
    wchar_t wbuf[BUFSIZE];
    std::mbstowcs(wbuf, dirname.c_str(), dirname.length() + 1);
    if (!CreateDirectoryW(wbuf, nullptr)) {
        return GetLastError();
    }
#else
    if (access(dirname.c_str(), F_OK) != 0) {
        if (errno == ENOENT) {
            if (mkdir(dirname.c_str(), 0777) != 0) {
                return errno;
            }
        } else {
            return errno;
        }
    }
#endif
    return 0;
}

std::uint64_t tid2u64(std::thread::id tid)
{
    std::ostringstream oss;
    oss << tid;
    return std::stoull(oss.str());
}
