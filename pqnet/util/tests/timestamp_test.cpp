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

    pqnet::uint t = 2500;
    auto time1 = pqnet::ms2SecAndNsec(t);
    std::printf("%dms is %ds+%dns.\n", t, time1.first, time1.second);
    auto time2 = pqnet::us2SecAndNsec(t);
    std::printf("%dus is %ds+%dns.\n", t, time2.first, time2.second);

    return 0;
}
