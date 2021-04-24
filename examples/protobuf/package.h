#ifndef PACKAGE_H
#define PACKAGE_H

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

#include <pqnet/util/random.h>

#define PROTOCOL_NO 0x48

inline std::uint32_t RandomUID() {
    return pqnet::Uint32(100000, 1000000);
}

inline std::string RandomName() {
    std::uint32_t len = pqnet::Uint32(4, 12 + 1);

    std::string name;

    for (std::uint32_t i = 0; i < len; ++i) {
        name.append(1, pqnet::Uint32('a', 'z' + 1));
    }

    return name;
}

inline std::uint32_t RandomLevel() {
    return pqnet::Uint32(1, 100);
}

class Package
{
public:
    Package(std::int32_t protocolNo) : dataLen(0) {
        std::memset(buf, 0, sizeof(buf));
        std::memcpy(buf, &protocolNo, sizeof(protocolNo));
        dataOffset = sizeof(protocolNo) + sizeof(dataLen);
    }
    void packInt(std::int32_t data) {
        dataLen += sizeof(std::int32_t);
        std::memcpy(buf + sizeof(std::int32_t), &dataLen, sizeof(dataLen));

        std::memcpy(buf + dataOffset, &data, sizeof(data));
        dataOffset += sizeof(data);
    }
    void packString(const std::string& data) {
        dataLen += data.length();
        std::memcpy(buf + sizeof(std::int32_t), &dataLen, sizeof(dataLen));

        std::strcpy(buf + dataOffset, data.c_str());
        dataOffset += data.length();
    }
    void clearData() {
        dataLen = 0;
        std::memcpy(buf + sizeof(std::int32_t), &dataLen, sizeof(dataLen));

        dataOffset = sizeof(std::int32_t) + sizeof(dataLen);
    }
    const char* p() const { return buf; }
    std::size_t length() const { return dataLen + 2 * sizeof(std::int32_t); }
private:
    std::int32_t dataLen;
    std::int32_t dataOffset;
    char buf[32];
};

#endif // PACKAGE_H
