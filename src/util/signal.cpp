#include <cerrno>
#include <cstring>

#include <map>

#include "logger.h"
#include "signal.h"

using namespace pqnet;

namespace {

std::map<int, pn_signal_func> sigbox;

void sighandler(int signum) {
    sigbox[signum]();
}

} // unnamed namespace

void pqnet::waitSig()
{
    for (auto sig : sigbox) {
        if (signal(sig.first, sighandler) == SIG_ERR) {
            ERROR(std::strerror(errno));
        }
    }
}

void pqnet::addSignal(int signum, pn_signal_func func)
{
    sigbox[signum] = func;
}

void pqnet::delSignal(int signum)
{
    if (sigbox.find(signum) != sigbox.end()) {
        sigbox.erase(signum);
    }
}
