#include <cerrno>
#include <cstring>

#include <map>

#include <signal.h>

#include "logger.h"
#include "signal.h"

using namespace pqnet;

std::map<int, pn_signal_func> pqnet::sigset;

void pqnet::sighandler(int signum) {
    sigset[signum]();
}

void pqnet::waitSig()
{
    for (auto sig : sigset) {
        if (signal(sig.first, sighandler) == SIG_ERR) {
            ERROR(std::strerror(errno));
        }
    }
}

void pqnet::addSignal(int signum, pn_signal_func func)
{
    sigset[signum] = func;
}

void pqnet::delSignal(int signum)
{
    if (sigset.find(signum) != sigset.end()) {
        sigset.erase(signum);
    }
}
