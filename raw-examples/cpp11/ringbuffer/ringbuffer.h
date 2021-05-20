#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <cstdint>
#include <cstring>
#include <string>

class RingBuffer
{
public:
    RingBuffer() : full(false), rIdx(0), wIdx(0) { std::memset(buf, 0, sizeof(buf)); }

    std::size_t rIndex() const { return rIdx; }
    std::size_t wIndex() const { return wIdx; }

    bool isFull() const { return full; }
    bool isReadable() const { return readableBytes() > 0; }
    bool isWritable() const { return writableBytes() > 0; }
    bool isReadable(std::size_t len) const { return readableBytes() >= len; }
    bool isWritable(std::size_t len) const { return writableBytes() >= len; }

    std::size_t readableBytes() const {
        if (rIdx < wIdx) {
            return wIdx - rIdx;
        } else if (rIdx > wIdx) {
            return (SIZE - rIdx) + wIdx;
        } else {
            return full ? SIZE : 0;
        }
    }
    std::size_t writableBytes() const {
        if (wIdx < rIdx) {
            return rIdx - wIdx;
        } else if (wIdx > rIdx) {
            return (SIZE - wIdx) + rIdx;
        } else {
            return full ? 0 : SIZE;
        }
    }

    std::string get(std::size_t len) {
        std::string result;

        if (!isReadable(len)) {
            return result;
        }

        if (rIdx < wIdx) {
            result.assign(buf + rIdx, buf + rIdx + len);
            rIdx += len;
        } else {
            if (rIdx == wIdx) {
                full = false;
            }

            std::size_t len1 = SIZE - rIdx;

            if (len <= len1) {
                result.assign(buf + rIdx, buf + rIdx + len);
                rIdx += len;
            } else {
                std::size_t len2 = len - len1;

                result.append(buf + rIdx, buf + rIdx + len1);
                result.append(buf, buf + len2);

                rIdx = len2;
            }
        }

        return result;
    }

    std::int8_t getInt8() { return getIntN<std::int8_t>(); }
    std::int16_t getInt16() { return getIntN<std::int16_t>(); }
    std::int32_t getInt32() { return getIntN<std::int32_t>(); }
    std::int64_t getInt64() { return getIntN<std::int64_t>(); }

    bool append(const char *data, std::size_t len) {
        if (!isWritable(len)) {
            return false;
        }

        if (wIdx < rIdx) {
            std::strncpy(buf + wIdx, data, len);
            wIdx += len;
        } else {
            std::size_t len1 = SIZE - wIdx;

            if (len <= len1) {
                std::strncpy(buf + wIdx, data, len);
                wIdx += len;
            } else {
                std::size_t len2 = len - len1;

                std::strncpy(buf + wIdx, data, len1);
                std::strncpy(buf, data + len1, len2);

                wIdx = len2;
            }
        }

        if (rIdx == wIdx) {
            full = true;
        }

        return true;
    }

    bool appendInt8(std::int8_t x) { return appendIntN<std::int8_t>(x); }
    bool appendInt16(std::int16_t x) { return appendIntN<std::int16_t>(x); }
    bool appendInt32(std::int32_t x) { return appendIntN<std::int32_t>(x); }
    bool appendInt64(std::int64_t x) { return appendIntN<std::int64_t>(x); }
private:
    template <typename T>
    T getIntN() {
        T result = T();
        std::size_t len = sizeof(T);

        if (!isReadable(len)) {
            return result;
        }

        if (rIdx < wIdx) {
            std::memcpy(&result, buf + rIdx, len);
            rIdx += len;
        } else {
            if (rIdx == wIdx) {
                full = false;
            }

            std::size_t len1 = SIZE - rIdx;

            if (len <= len1) {
                std::memcpy(&result, buf + rIdx, len);
                rIdx += len;
            } else {
                std::size_t len2 = len - len1;

                std::memcpy(&result, buf + rIdx, len1);
                std::memcpy(&result + len1, buf, len2);

                rIdx = len2;
            }
        }

        return result;
    }

    template <typename T>
    bool appendIntN(T x) {
        std::size_t len = sizeof(T);

        if (!isWritable(len)) {
            return false;
        }

        if (wIdx < rIdx) {
            std::memcpy(buf + wIdx, &x, len);
            wIdx += len;
        } else {
            std::size_t len1 = SIZE - wIdx;

            if (len <= len1) {
                std::memcpy(buf + wIdx, &x, len);
                wIdx += len;
            } else {
                std::size_t len2 = len - len1;

                auto s = reinterpret_cast<const char*>(&x);

                std::memcpy(buf + wIdx, s, len1);
                std::memcpy(buf, s + len1, len2);

                wIdx = len2;
            }
        }

        if (rIdx == wIdx) {
            full = true;
        }

        return true;
    }
private:
    static constexpr std::size_t SIZE = 1024;
private:
    char buf[SIZE];
    bool full;
    std::size_t rIdx;
    std::size_t wIdx;
};

#endif // RING_BUFFER_H
