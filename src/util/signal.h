#ifndef PQNET_UTIL_SIGNAL_H
#define PQNET_UTIL_SIGNAL_H

#include <map>

#include <signal.h>

#include "types.h"

namespace pqnet
{

class Signal
{
public:
    Signal();
    ~Signal();
    void waitSig();
    // Add or Update signal(s)
    void addSignal(int signum, pn_signal_func func);
    // Delete signal(s)
    void delSignal(int signum);
private:
    std::map<int, pn_signal_func> sigset;
};

} // namespace pqnet

#endif // PQNET_UTIL_SIGNAL_H
