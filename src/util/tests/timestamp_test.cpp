#include <iostream>

#include "../timestamp.h"

int main()
{
    auto ts = pqnet::now();

    std::cout << ts.Second() << std::endl;
    std::cout << ts.Millisecond() << std::endl;
    std::cout << ts.Microsecond() << std::endl;
    std::cout << ts.Nanosecond() << std::endl;

    std::cout << ts.Int10() << std::endl;
    std::cout << ts.Int13() << std::endl;
    std::cout << ts.Int16() << std::endl;
    std::cout << ts.Int19() << std::endl;

    auto time = pqnet::ms2SecAndNsec(2500);
    cout << time.first << 's' << ' ' << time.second << 'ns\n';

    return 0;
}
