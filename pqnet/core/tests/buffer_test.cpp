#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <pqnet/core/buffer.h>

void output(const pqnet::Buffer& buffer) {
    std::printf("RB: %4lu WB: %4lu\n", buffer.readableBytes(), buffer.writableBytes());
}

int main()
{
    pqnet::Buffer buffer;
    output(buffer);

    const char *data = "Hello pqnet!";
    buffer.append(data, std::strlen(data));
    output(buffer);

    buffer.appendInt32(1024);
    output(buffer);

    auto f = std::fopen("/proc/version", "r");
    buffer.readFrom(fileno(f));
    output(buffer);
    if (std::fclose(f) != 0) {
        std::exit(EXIT_FAILURE);
    }

    auto str = buffer.get(std::strlen(data));
    std::cout << str << std::endl;
    output(buffer);

    auto x = buffer.getInt32();
    std::cout << x << std::endl;
    output(buffer);

    buffer.writeTo(fileno(stdout), 256);
    output(buffer);

    buffer.makeSpace(128);
    output(buffer);

    return 0;
}
