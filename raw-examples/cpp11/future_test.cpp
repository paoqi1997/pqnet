#include <chrono>
#include <future>
#include <iostream>
#include <thread>

using std::chrono::duration_cast;
using std::chrono::seconds;
using std::chrono::system_clock;
using std::chrono::time_point;

void func(std::promise<bool> *pObj) {
    std::cout << "Hello std::future!" << std::endl;
    pObj->set_value(true);
}

int getResultFromDB() {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return 1;
}

int getResultFromFile() {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return 2;
}

int main()
{
    // Part 1: For std::future & std::promise
    std::promise<bool> promiseObj;
    std::future<bool> futureObj = promiseObj.get_future();
    std::thread t(func, &promiseObj);
    if (futureObj.get()) {
        std::cout << "True" << std::endl;
    } else {
        std::cout << "False" << std::endl;
    }
    if (t.joinable()) {
        t.join();
    }

    // Part 2: For std::async & std::launch
    time_point<system_clock> t1 = system_clock::now();

    std::future<int> fObj = std::async(std::launch::async, getResultFromDB);
    int fileRes = getResultFromFile();
    int dbRes = fObj.get();

    time_point<system_clock> t2 = system_clock::now();
    // just need 3s.
    std::cout << fileRes + dbRes << " " << duration_cast<seconds>(t2 - t1).count() << std::endl;

    // Part 3: For std::packaged_task
    std::packaged_task<int(int)> task([](int x){ return x * x; });
    auto fObject = task.get_future();
    std::thread td(std::move(task), 2);
    td.join();
    std::cout << fObject.get() << std::endl;

    return 0;
}
