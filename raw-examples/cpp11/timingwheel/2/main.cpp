#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

#include "timermanager.h"

using std::cout;
using std::endl;

void printCurrTime() {
#ifdef _WIN32
    std::system("echo %date% %time%");
#else
    std::system("date +\"%F %T\"");
#endif
}

int main()
{
    int count = 0;
    std::size_t tick = 20; // 50Hz

    TimerManager tm;
    TimerId myID;

    auto cb = [&](void *arg){
        printCurrTime();
        cout << static_cast<char*>(arg) << endl;
        if (++count == 10) {
            tm.delTimer(myID);
        }
    };

    printCurrTime();
    cout << "Start Timing!" << endl;

    tm.addTimer("Timer", cb, const_cast<char*>("Timer!"), 6000);
    myID = tm.addTimer("Ticker", cb, const_cast<char*>("Ticker!"), 3000, 1000);

    for (;;) {
        std::this_thread::sleep_for(std::chrono::milliseconds(tick));
        tm.handle();
    }

    return 0;
}
