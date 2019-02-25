#include <cerrno>
#include <cstring>

#include "logger.h"
#include "signal.h"

using namespace pqnet;

Signal::Signal()
{

}

Signal::~Signal()
{

}

void Signal::waitSig()
{
    for (auto sig : sigset) {
        if (signal(sig.first, sig.second) == SIG_ERR) {
            ERROR(std::strerror(errno));
        }
    }
}

void Signal::addSignal(int signum, pn_signal_func func)
{
    sigset[signum] = func;
}

void Signal::delSignal(int signum)
{
    if (sigset.find(signum) != sigset.end()) {
        sigset.erase(signum);
    }
}
