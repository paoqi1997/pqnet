#include <cerrno>
#include <cstring>
#include <map>

#include "logger.h"
#include "signal.h"

using namespace pqnet;

namespace {

std::map<int, pn_signal_func_t> sigbox;
void sighandler(int signum) {
    sigbox[signum]();
}

} // unnamed namespace

void pqnet::waitSig()
{
    for (auto sig : sigbox) {
        if (std::signal(sig.first, sighandler) == SIG_ERR) {
            ERROR(std::strerror(errno));
        }
    }
}

void pqnet::addSignal(int signum, pn_signal_func_t func)
{
    sigbox[signum] = func;
}

void pqnet::delSignal(int signum)
{
    if (sigbox.find(signum) != sigbox.end()) {
        sigbox.erase(signum);
        if (std::signal(signum, SIG_DFL) == SIG_ERR) {
            ERROR(std::strerror(errno));
        }
    }
}
