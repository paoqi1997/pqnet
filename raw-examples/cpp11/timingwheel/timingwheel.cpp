#include "timingwheel.h"

Tim::Tim(const timerCallBack& cb, void *_arg, uint _interval)
    : timercb(cb), arg(_arg), interval(_interval), valid(true)
{
    id = reinterpret_cast<TimerId>(this);
}

Tim* TimingWheel::addTimer(const timerCallBack& cb, void *arg, uint expiration, uint interval)
{
    Tim tim(cb, arg, interval);
    std::size_t ticks = expiration < tick ? 1 : expiration / tick;
    std::size_t round = ticks / SlotNum;
    std::size_t _slotIdx = (slotIdx + ticks) % SlotNum;
    tim.Round = round;
    tim.SlotIdx = _slotIdx;
    auto& slot = slots[_slotIdx];
    auto it = slot.insert(slot.end(), tim);
    return &*it;
}

void TimingWheel::delTimer(Tim *tim)
{
    std::list<Tim>& slot = slots[tim->SlotIdx];
    for (auto it = slot.begin(); it != slot.end(); ++it) {
        if (it->Id() == tim->Id()) {
            it->invalidate();
            break;
        }
    }
}

void TimingWheel::handle()
{
    std::list<Tim>& currSlot = slots[slotIdx];
    for (auto it = currSlot.begin(); it != currSlot.end(); ) {
        if (it->Round > 0) {
            --it->Round;
            ++it;
        } else {
            it->run();
            if (!it->isValid()) {
                it = currSlot.erase(it);
                continue;
            }
            if (it->isPeriodic()) {
                std::size_t expiration = it->Interval();
                std::size_t ticks = expiration < tick ? 1 : expiration / tick;
                std::size_t round = ticks / SlotNum;
                std::size_t _slotIdx = (slotIdx + ticks) % SlotNum;
                it->Round = round;
                it->SlotIdx = _slotIdx;
                // 将位于currSlot的定时器移至nextSlot
                auto tmpIt = std::next(it, 1);
                auto& nextSlot = slots[_slotIdx];
                nextSlot.splice(nextSlot.end(), currSlot, it);
                it = tmpIt;
            } else {
                it = currSlot.erase(it);
            }
        }
    }
    slotIdx = (slotIdx + 1) % SlotNum;
}
