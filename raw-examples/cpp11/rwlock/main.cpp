#include <cstdio>
#include <iostream>
#include <thread>
#include <vector>

#include "rwlock.h"

using std::cout;
using std::endl;

RWLock rwlock;

void buyTicket(std::size_t *remain);
void checkTicket(std::size_t *remain);

int main()
{
    std::size_t remain = 100;

    std::vector<std::thread> buyers;
    std::vector<std::thread> checkers;

    for (int i = 0; i < 100; ++i) {
        if (i % 10 == 0) {
            buyers.emplace_back(buyTicket, &remain);
        } else {
            checkers.emplace_back(checkTicket, &remain);
        }
    }

    for (auto& buyer : buyers) {
        buyer.join();
    }

    for (auto& checker : checkers) {
        checker.join();
    }

    return 0;
}

void buyTicket(std::size_t *remain) {
    WriteGuard guard(rwlock);
    *remain -= 10;
    cout << "buy 10 tickets" << endl;
}

void checkTicket(std::size_t *remain) {
    ReadGuard guard(rwlock);
    std::printf("remain: %zu\n", *remain);
}
