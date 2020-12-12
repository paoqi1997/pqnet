#ifndef PACKAGE_H
#define PACKAGE_H

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

#include <pqnet/util/random.h>

#define _1_0_KB 1024
#define _1_5_KB 1536
#define _2_0_KB 2048

#define PROTOCOL_NO 0x64

#define ASCII_PRINT_FIRST 32  // ' '
#define ASCII_PRINT_LAST  126 // '~'

class Package
{
public:
    Package(std::int32_t protocolNo) : dataLen(0) {
        std::memset(buf, 0, sizeof(buf));
        std::memcpy(buf, &protocolNo, sizeof(protocolNo));
    }
    void refreshData() {
        dataLen = pqnet::Uint32(_1_0_KB, _1_5_KB);
        std::memcpy(buf + sizeof(std::int32_t), &dataLen, sizeof(dataLen));

        char *data = buf + 2 * sizeof(std::int32_t);
        for (std::int32_t i = 0; i < dataLen; ++i) {
            data[i] = pqnet::Uint32(ASCII_PRINT_FIRST, ASCII_PRINT_LAST + 1);
        }
    }
    const char* p() const { return buf; }
    std::size_t length() const { return dataLen + 2 * sizeof(std::int32_t); }
    std::string begin3() const {
        auto begin = buf + 2 * sizeof(std::int32_t);
        return std::string(begin, begin + 3);
    }
    std::string end3() const {
        auto end = buf + dataLen + 2 * sizeof(std::int32_t);
        return std::string(end - 3, end);
    }
    std::size_t dataLength() const { return dataLen; }
private:
    std::int32_t dataLen;
    char buf[_2_0_KB];
};

#endif // PACKAGE_H
