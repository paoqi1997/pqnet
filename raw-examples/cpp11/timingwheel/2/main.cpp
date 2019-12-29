#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

#include "timermanager.h"

using std::cout;
using std::endl;

void printCurrTime() {
#ifdef WIN32
    std::system("echo %date% %time%");
#else
    std::system("date +\"%F %T\"");
#endif
}

int main()
{
    int count = 0;
    std::size_t tick = 50;
    TimerManager tm;
    TimerNode *node;
    auto cb = [&](void *arg){
        printCurrTime();
        std::cout << static_cast<char*>(arg) << std::endl;
        if (++count == 10) {
            tm.delTimer(node);
        }
    };
    printCurrTime();
    std::cout << "Start Timing!" << std::endl;
    //tm.addTimer(cb, const_cast<char*>("Timer!"), 6000);
    node = tm.addTimer(cb, const_cast<char*>("Ticker!"), 3000, 1000);
    for (;;) {
        std::this_thread::sleep_for(std::chrono::milliseconds(tick));
        tm.handle();
    }
    return 0;
}
