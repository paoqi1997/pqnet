#include <iostream>

#include <pqnet/util/timestamp.h>

using std::cout;
using std::endl;

int main()
{
    auto ts = pqnet::now();

    cout << ts.Second() << endl;
    cout << ts.Millisecond() << endl;
    cout << ts.Microsecond() << endl;
    cout << ts.Nanosecond() << endl;

    cout << ts.Int10() << endl;
    cout << ts.Int13() << endl;
    cout << ts.Int16() << endl;
    cout << ts.Int19() << endl;

    auto ts2 = pqnet::oneday({2020, 10, 1, 10, 30, 0});

    cout << ts2.Int10() << endl;
    cout << ts2.Int13() << endl;
    cout << ts2.Int16() << endl;
    cout << ts2.Int19() << endl;

    pqnet::uint t = 2500;
    auto time1 = pqnet::ms2SecAndNsec(t);
    std::printf("%dms is %ds+%dns.\n", t, time1.first, time1.second);
    auto time2 = pqnet::us2SecAndNsec(t);
    std::printf("%dus is %ds+%dns.\n", t, time2.first, time2.second);

    return 0;
}
