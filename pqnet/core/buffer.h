#ifndef PQNET_CORE_BUFFER_H
#define PQNET_CORE_BUFFER_H

#include <cstdint>

#include <string>
#include <vector>

namespace pqnet
{

class Buffer
{
public:
    // interval: (-∞, -1)
    enum EvCode {
        BUFFER_SIZE_MAXIMUM = -2
    };
    Buffer(std::size_t size = 1024);
    bool doubleSize();
    void vacate();
    void makeSpace(std::size_t len);
    bool isReadable() const { return readableBytes() > 0; }
    bool isWritable() const { return writableBytes() > 0; }
    bool isReadable(std::size_t len) const { return readableBytes() >= len; }
    bool isWritable(std::size_t len) const { return writableBytes() >= len; }
    std::size_t frontBytes() const { return readerIndex; }
    std::size_t readableBytes() const { return writerIndex - readerIndex; }
    std::size_t writableBytes() const { return buf.size() - writerIndex; }
    // Buffer -> Host/Net
    ssize_t writeTo(int fd, std::size_t len);
    std::string get(std::size_t len);
    std::int8_t getInt8();
    std::int16_t getInt16();
    std::int32_t getInt32();
    std::int64_t getInt64();
    // Host/Net -> Buffer
    ssize_t readFrom(int fd);
    void append(const char *data, std::size_t len);
    void appendInt8(std::int8_t x);
    void appendInt16(std::int16_t x);
    void appendInt32(std::int32_t x);
    void appendInt64(std::int64_t x);
private:
    char* begin() { return buf.data(); }
    const char* begin() const { return buf.data(); }
    char* beginRead() { return begin() + readerIndex; }
    const char* beginRead() const { return begin() + readerIndex; }
    char* beginWrite() { return begin() + writerIndex; }
    const char* beginWrite() const { return begin() + writerIndex; }
private:
    static const std::size_t MAXSIZE = 8192;
private:
    std::vector<char> buf;
    std::size_t readerIndex;
    std::size_t writerIndex;
};

} // namespace pqnet

#endif // PQNET_CORE_BUFFER_H
