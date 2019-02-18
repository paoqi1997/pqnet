#include <iostream>

#include <cstdio>
#include <cstring>

#include "../../buffer.h"

void output(const pqnet::Buffer& buffer) {
    std::cout << buffer.readableBytes() << ' ' << buffer.writableBytes() << std::endl;
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
    buffer.readFrom(fileno(f), 192);
    output(buffer);

    auto str = buffer.get(std::strlen(data));
    std::cout << str << std::endl;
    output(buffer);

    auto x = buffer.getInt32();
    std::cout << x << std::endl;
    output(buffer);

    buffer.writeTo(fileno(stdout), 192);
    output(buffer);

    buffer.makeSpace(128);
    output(buffer);

    return 0;
}
