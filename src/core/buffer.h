#ifndef PQNET_UTIL_BUFFER_H
#define PQNET_UTIL_BUFFER_H

#include <cstdint>

#include <string>
#include <vector>

#include <unistd.h>

namespace pqnet
{

class Buffer
{
public:
    Buffer(std::size_t size = 1024);
    ~Buffer();
    void makeSpace(std::size_t len);
    bool isReadable(std::size_t len) const;
    bool isWritable(std::size_t len) const;
    std::size_t idleBytes() const { return readerIndex; }
    std::size_t readableBytes() const { return writerIndex - readerIndex; }
    std::size_t writableBytes() const { return buf.size() - writerIndex; }
    // Buffer -> Host/Network
    ssize_t writeTo(int fd, std::size_t len);
    std::string get(std::size_t len);
    std::int8_t getInt8();
    std::int16_t getInt16();
    std::int32_t getInt32();
    std::int64_t getInt64();
    // Host/Network -> Buffer
    ssize_t readFrom(int fd, std::size_t len);
    void append(const char *data, std::size_t len);
    void appendInt8(std::int8_t x);
    void appendInt16(std::int16_t x);
    void appendInt32(std::int32_t x);
    void appendInt64(std::int64_t x);
private:
    char* begin() { return &*buf.begin(); }
    const char* begin() const { return &*buf.begin(); }
    char* beginRead() { return begin() + readerIndex; }
    const char* beginRead() const { return begin() + readerIndex; }
    char* beginWrite() { return begin() + writerIndex; }
    const char* beginWrite() const { return begin() + writerIndex; }
private:
    std::vector<char> buf;
    std::size_t readerIndex;
    std::size_t writerIndex;
};

} // namespace pqnet

#endif // PQNET_UTIL_BUFFER_H