#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <cerrno>
#include <cstring>
#include <iostream>
#include <queue>
#include <vector>

#include <pthread.h>

class Mutex
{
public:
    Mutex() {
        if (pthread_mutex_init(&mtx, nullptr) != 0) {
            std::cout << std::strerror(errno) << std::endl;
        }
    }
    ~Mutex() {
        if (pthread_mutex_destroy(&mtx) != 0) {
            std::cout << std::strerror(errno) << std::endl;
        }
    }
    void lock() {
        if (pthread_mutex_lock(&mtx) != 0) {
            std::cout << std::strerror(errno) << std::endl;
        }
    }
    void unlock() {
        if (pthread_mutex_unlock(&mtx) != 0) {
            std::cout << std::strerror(errno) << std::endl;
        }
    }
    pthread_mutex_t* getPtr() { return &mtx; }
private:
    pthread_mutex_t mtx;
};

class Condition
{
public:
    Condition() {
        if (pthread_cond_init(&cond, nullptr) != 0) {
            std::cout << std::strerror(errno) << std::endl;
        }
    }
    ~Condition() {
        if (pthread_cond_destroy(&cond) != 0) {
            std::cout << std::strerror(errno) << std::endl;
        }
    }
    void wait() {
        if (pthread_cond_wait(&cond, mtx.getPtr()) != 0) {
            std::cout << std::strerror(errno) << std::endl;
        }
    }
    void notify() {
        if (pthread_cond_signal(&cond) != 0) {
            std::cout << std::strerror(errno) << std::endl;
        }
    }
    void notifyAll() {
        if (pthread_cond_broadcast(&cond) != 0) {
            std::cout << std::strerror(errno) << std::endl;
        }
    }
    void lock() { mtx.lock(); }
    void unlock() { mtx.unlock(); }
private:
    pthread_cond_t cond;
    Mutex mtx;
};

void* routine(void *arg);

struct Task
{
    void run() { func(arg); }
    void (*func)(void*);
    void *arg;
};

class ThreadPool
{
public:
    ThreadPool(std::size_t threadNumber);
    ~ThreadPool();
    Condition& Cond() { return cond; }
    void run();
    void shutdown() { running = addable = false; }
    void addTask(Task task);
    Task take();
    bool isRunning() const { return running; }
    bool isIdle() const { return taskqueue.empty(); }
private:
    bool running;
    bool addable;
    std::size_t tn;
    Condition cond;
    std::queue<Task> taskqueue;
    std::vector<pthread_t> pool;
};

#endif // THREAD_POOL_H
