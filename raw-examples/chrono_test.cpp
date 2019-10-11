#include <chrono>
#include <iostream>

using namespace std::chrono;

int main()
{
    time_point<system_clock> t1 = system_clock::now();
    std::cout << "Hello World!" << std::endl;
    time_point<system_clock> t2 = system_clock::now();

    auto _duration = duration_cast<microseconds>(t2 - t1);
    std::cout << _duration.count() << std::endl;

    using day_type = duration<int, std::ratio<60 * 60 * 24>>;
    time_point<system_clock, day_type> tp = time_point_cast<day_type>(system_clock::now());
    std::cout << tp.time_since_epoch().count() << std::endl;

    return 0;
}
