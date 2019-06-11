#include <iostream>

#include <pqnet/util/random.h>

int main()
{
    for (std::size_t i = 0; i < 10; ++i) {
        std::cout << pqnet::Uint32(0, 10) << ' ';
    }
    std::cout << std::endl;
    for (std::size_t i = 0; i < 10; ++i) {
        std::cout << pqnet::Uint32(10, 0) << ' ';
    }
    std::cout << std::endl;
    return 0;
}
