#include <chrono>
#include <iostream>

using std::cout;
using std::endl;

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::system_clock;
using std::chrono::time_point;
using std::chrono::time_point_cast;

using  us_type = std::chrono::microseconds;
using day_type = duration<int, std::ratio<60*60*24>>;

int main()
{
    time_point<system_clock> t1 = system_clock::now();
    cout << "Hello World!" << endl;
    time_point<system_clock> t2 = system_clock::now();

    us_type myDuration = duration_cast<us_type>(t2 - t1);
    cout << myDuration.count() << endl;

    time_point<system_clock, day_type> tp = time_point_cast<day_type>(system_clock::now());
    cout << tp.time_since_epoch().count() << endl;

    return 0;
}
