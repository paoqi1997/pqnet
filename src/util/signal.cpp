#include <cerrno>
#include <cstring>

#include "logger.h"
#include "signal.h"

namespace pqnet
{

Signal::Signal() : func(nullptr)
{

}

Signal::Signal(pn_signal_func _func) : func(_func)
{

}

Signal::~Signal()
{

}

void Signal::waitSig()
{
    if (func != nullptr) {
        for (auto sig : sigset) {
            if (signal(sig, func) == SIG_ERR) {
                ERROR(std::strerror(errno));
            }
        }
    }
}

void Signal::addSignal(int signum)
{
    if (sigset.find(signum) == sigset.end()) {
        sigset.insert(signum);
    }
}

void Signal::delSignal(int signum)
{
    if (sigset.find(signum) != sigset.end()) {
        sigset.erase(signum);
    }
}

} // namespace pqnet
