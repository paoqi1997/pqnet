#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

#include "timingwheel.h"

using std::cout;
using std::endl;

void printCurrTime() {
    std::system("echo %date% %time%");
}

int main()
{
    int count = 0;
    TimingWheel tw(10);
    Tim *tim;
    auto cb = [&](void *arg){
        printCurrTime();
        std::cout << static_cast<char*>(arg) << std::endl;
        if (++count == 10) {
            tw.delTimer(tim);
        }
    };
    printCurrTime();
    std::cout << "Start Timing!" << std::endl;
    tw.addTimer(cb, const_cast<char*>("Timer!"), 6000);
    tim = tw.addTimer(cb, const_cast<char*>("Ticker!"), 3000, 1000);
    for (;;) {
        std::this_thread::sleep_for(std::chrono::milliseconds(tw.getTick()));
        tw.handle();
    }
    return 0;
}
