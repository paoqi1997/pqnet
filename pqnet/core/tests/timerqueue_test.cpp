#include <cstdlib>
#include <iostream>

#include <pqnet/core/eventloop.h>

void printCurrTime() {
    std::system("date +\"%F %T\"");
}

int main()
{
    int count = 0;
    pqnet::TimerId myID;
    pqnet::EventLoop looper;
    auto cb = [&](void *arg){
        printCurrTime();
        std::cout << static_cast<char*>(arg) << std::endl;
        if (++count == 10) {
            looper.cancel(myID);
        }
    };
    printCurrTime();
    std::cout << "Start Timing!" << std::endl;
    looper.runAfter(cb, const_cast<char*>("Timer!"), 6000);
    myID = looper.runEvery(cb, const_cast<char*>("Ticker!"), 3000, 1000);
    looper.loop();
    return 0;
}
