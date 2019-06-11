#include <iostream>

#include <pqnet/util/timestamp.h>

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

    auto time1 = pqnet::ms2SecAndNsec(2500);
    std::cout << time1.first << 's' << ' ' << time1.second << "ns\n";
    auto time2 = pqnet::us2SecAndNsec(2500);
    std::cout << time2.first << 's' << ' ' << time2.second << "ns\n";

    return 0;
}
