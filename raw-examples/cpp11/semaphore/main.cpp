#include <cstring>
#include <iostream>
#include <thread>

#include "semaphore.h"

using std::cout;
using std::endl;

bool useSem = false;
Semaphore sem1(0), sem2(0);

void func1() {
    if (useSem) {
        sem2.wait();
    }
    cout << "FANTASY" << endl;
}

void func2() {
    if (useSem) {
        sem1.wait();
    }
    cout << "DARK" << endl;
    if (useSem) {
        sem2.post();
    }
}

void func3() {
    cout << "DEEP" << endl;
    if (useSem) {
        sem1.post();
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::printf("Usage: app <useSem>\n");
        return 1;
    }

    useSem = std::atoi(argv[1]);

    std::thread t1(func1);
    std::thread t2(func2);
    std::thread t3(func3);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
