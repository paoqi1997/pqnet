#include <algorithm>
#include <cstring>

#include <unistd.h>

#include "../util/logger.h"
#include "buffer.h"

using namespace pqnet;

Buffer::Buffer(std::size_t size) : buf(size), readerIndex(0), writerIndex(0)
{

}

bool Buffer::doubleSize()
{
    std::size_t capacity = buf.capacity();
    if (capacity < MAXSIZE) {
        buf.resize(2 * capacity);
        return true;
    } else {
        DEBUG("the size of the buffer is maximum.");
        return false;
    }
}

void Buffer::vacate()
{
    std::size_t rbs = this->readableBytes();
    std::copy(this->beginRead(), this->beginWrite(), begin());
    readerIndex = 0;
    writerIndex = readerIndex + rbs;
}

void Buffer::makeSpace(std::size_t len)
{
    if (len > writableBytes() + frontBytes()) {
        this->doubleSize();
    } else {
        this->vacate();
    }
}

// Buffer -> Host/Net
ssize_t Buffer::writeTo(int fd, std::size_t len)
{
    ssize_t num;
    std::size_t rbs = this->readableBytes();
    if (len <= rbs) {
        num = write(fd, this->beginRead(), len);
        if (num > 0) {
            readerIndex += num;
        }
    } else {
        num = write(fd, this->beginRead(), rbs);
        if (num > 0) {
            readerIndex += num;
        }
    }
    return num;
}

std::string Buffer::get(std::size_t len)
{
    std::string result;
    std::size_t rbs = this->readableBytes();
    if (len <= rbs) {
        result.assign(this->beginRead(), this->beginRead() + len);
        readerIndex += len;
    } else {
        result.assign(this->beginRead(), this->beginRead() + rbs);
        readerIndex += rbs;
    }
    return result;
}

std::int8_t Buffer::getInt8()
{
    std::int8_t result;
    if (this->isReadable(sizeof(std::int8_t))) {
        std::memcpy(&result, this->beginRead(), sizeof(std::int8_t));
        readerIndex += sizeof(std::int8_t);
    }
    return result;
}

std::int16_t Buffer::getInt16()
{
    std::int16_t result;
    if (this->isReadable(sizeof(std::int16_t))) {
        std::memcpy(&result, this->beginRead(), sizeof(std::int16_t));
        readerIndex += sizeof(std::int16_t);
    }
    return result;
}

std::int32_t Buffer::getInt32()
{
    std::int32_t result;
    if (this->isReadable(sizeof(std::int32_t))) {
        std::memcpy(&result, this->beginRead(), sizeof(std::int32_t));
        readerIndex += sizeof(std::int32_t);
    }
    return result;
}

std::int64_t Buffer::getInt64()
{
    std::int64_t result;
    if (this->isReadable(sizeof(std::int64_t))) {
        std::memcpy(&result, this->beginRead(), sizeof(std::int64_t));
        readerIndex += sizeof(std::int64_t);
    }
    return result;
}

// Host/Net -> Buffer
ssize_t Buffer::readFrom(int fd)
{
    this->vacate();
    if (!isWritable()) {
        this->doubleSize();
    }

    ssize_t num = read(fd, this->beginWrite(), writableBytes());
    if (num > 0) {
        writerIndex += num;
        if (!isWritable()) {
            if (doubleSize()) {
                ssize_t n = readFrom(fd);
                if (n > 0) {
                    num += n;
                }
            }
        }
    }
    return num;
}

void Buffer::append(const char *data, std::size_t len)
{
    if (!isWritable(len)) {
        this->makeSpace(len);
    }
    std::copy(data, data + len, this->beginWrite());
    writerIndex += len;
}

void Buffer::appendInt8(std::int8_t x)
{
    auto s = reinterpret_cast<const char*>(&x);
    std::size_t len = sizeof(std::int8_t);
    if (!isWritable(len)) {
        this->makeSpace(len);
    }
    std::copy(s, s + len, this->beginWrite());
    writerIndex += len;
}

void Buffer::appendInt16(std::int16_t x)
{
    auto s = reinterpret_cast<const char*>(&x);
    std::size_t len = sizeof(std::int16_t);
    if (!isWritable(len)) {
        this->makeSpace(len);
    }
    std::copy(s, s + len, this->beginWrite());
    writerIndex += len;
}

void Buffer::appendInt32(std::int32_t x)
{
    auto s = reinterpret_cast<const char*>(&x);
    std::size_t len = sizeof(std::int32_t);
    if (!isWritable(len)) {
        this->makeSpace(len);
    }
    std::copy(s, s + len, this->beginWrite());
    writerIndex += len;
}

void Buffer::appendInt64(std::int64_t x)
{
    auto s = reinterpret_cast<const char*>(&x);
    std::size_t len = sizeof(std::int64_t);
    if (!isWritable(len)) {
        this->makeSpace(len);
    }
    std::copy(s, s + len, this->beginWrite());
    writerIndex += len;
}
