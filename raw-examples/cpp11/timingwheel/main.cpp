#include "timingwheel.h"

#include <chrono>
#include <iostream>
#include <thread>

using std::cout;
using std::endl;

int main()
{
    int count = 0;
    std::size_t tick = 10;
    TimingWheel tw(tick);
    Tim *tim;
    auto cb = [&](void *arg){
        cout << "Hello TimingWheel!" << endl;
        if (++count == 10) {
            tw.delTimer(tim);
        }
    };
    tim = tw.addTimer(cb, nullptr, 3000, 1000);
    for (;;) {
        std::this_thread::sleep_for(std::chrono::milliseconds(tick));
        tw.handle();
    }
    return 0;
}
