#ifndef PQNET_UTIL_SIGNAL_H
#define PQNET_UTIL_SIGNAL_H

#include <set>

#include <signal.h>

#include "types.h"

namespace pqnet
{

class Signal
{
public:
    Signal();
    Signal(pn_signal_func _func);
    ~Signal();
    void waitSig();
    void addSignal(int signum);
    void delSignal(int signum);
    void setHandler(pn_signal_func _func) { func = _func; }
private:
    pn_signal_func func;
    std::set<int> sigset;
};

} // namespace pqnet

#endif // PQNET_UTIL_SIGNAL_H
