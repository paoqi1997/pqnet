#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

#include <pqnet/core/timerqueue.h>

void printCurrTime() {
    std::system("date +\"%F %T\"");
}

int main()
{
    int count = 0;
    std::size_t tick = 20; // 50Hz
    pqnet::TimerQueue tq;
    pqnet::TimerId myID;
    auto cb = [&](void *arg){
        printCurrTime();
        std::cout << static_cast<char*>(arg) << std::endl;
        if (++count == 10) {
            tq.delTimer(myID);
        }
    };
    printCurrTime();
    std::cout << "Start Timing!" << std::endl;
    tq.addTimer(cb, const_cast<char*>("Timer!"), 6000);
    myID = tq.addTimer(cb, const_cast<char*>("Ticker!"), 3000, 1000);
    for (;;) {
        std::this_thread::sleep_for(std::chrono::milliseconds(tick));
        tq.handle();
    }
    return 0;
}
