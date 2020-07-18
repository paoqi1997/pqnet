#include <utility>

#include "../platform/base.h"
#include "eventloopthread.h"

using namespace pqnet;

EventLoopThread::EventLoopThread()
{

}

void EventLoopThread::start()
{
    std::unique_ptr<std::thread> tmpthd(
        new std::thread([this]{
            looper.loop();
        })
    );

    thd = std::move(tmpthd);
}

void EventLoopThread::join()
{
    if (thd->joinable()) {
        thd->join();
    }
}

std::size_t EventLoopThread::getId() const
{
    return tid2u64(thd->get_id());
}
