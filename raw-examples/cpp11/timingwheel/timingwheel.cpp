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
    slots[_slotIdx].push_back(tim);
    return &slots[_slotIdx].back();
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
            Tim tim = *it;
            tim.run();
            if (!it->isValid()) {
                it = currSlot.erase(it);
                continue;
            }
            if (tim.isPeriodic()) {
                std::size_t expiration = tim.Interval();
                std::size_t ticks = expiration < tick ? 1 : expiration / tick;
                std::size_t round = ticks / SlotNum;
                std::size_t _slotIdx = (slotIdx + ticks) % SlotNum;
                tim.Round = round;
                tim.SlotIdx = _slotIdx;
                it = currSlot.erase(it);
                slots[_slotIdx].push_back(tim);
            } else {
                it = currSlot.erase(it);
            }
        }
    }
    slotIdx = (slotIdx + 1) % SlotNum;
}
