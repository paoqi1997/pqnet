#ifndef RWLOCK_H
#define RWLOCK_H

#include <atomic>
#include <condition_variable>
#include <mutex>

class RWLock
{
public:
    RWLock() : wflag(false), rcnt(0), wcnt(0) {}
    RWLock(const RWLock&) = delete;
    /**
     * 读者使用读锁，如果当前没有写者，读者立即获得读锁
     * 否则读者等待，直到没有写者
     */
    void rdLock() {
        std::unique_lock<std::mutex> lk(mtx);
        rcond.wait(lk, [this]{ return wcnt == 0; });
        ++rcnt;
    }
    void rdUnlock() {
        std::unique_lock<std::mutex> lk(mtx);
        if (--rcnt == 0 && wcnt > 0) {
            wcond.notify_one();
        }
    }
    /**
     * 写者使用写锁，如果当前没有读者，也没有其他写者，写者立即获得写锁
     * 否则写者等待，直到没有读者和其他写者
     */
    void wrLock() {
        std::unique_lock<std::mutex> lk(mtx);
        ++wcnt;
        wcond.wait(lk, [this]{ return rcnt == 0 && !wflag; });
        wflag = true;
    }
    void wrUnlock() {
        std::unique_lock<std::mutex> lk(mtx);
        if (--wcnt == 0) {
            rcond.notify_all();
        } else {
            wcond.notify_one();
        }
        wflag = false;
    }
private:
    std::atomic<bool> wflag;
    std::atomic<std::size_t> rcnt;
    std::atomic<std::size_t> wcnt;
    std::mutex mtx;
    std::condition_variable rcond;
    std::condition_variable wcond;
};

class ReadGuard
{
public:
    explicit ReadGuard(RWLock& _rwlock) : rwlock(_rwlock) {
        rwlock.rdLock();
    }
    ~ReadGuard() {
        rwlock.rdUnlock();
    }
private:
    RWLock& rwlock;
};

class WriteGuard
{
public:
    explicit WriteGuard(RWLock& _rwlock) : rwlock(_rwlock) {
        rwlock.wrLock();
    }
    ~WriteGuard() {
        rwlock.wrUnlock();
    }
private:
    RWLock& rwlock;
};

#endif // RWLOCK_H
