#ifndef PQNET_UTIL_SIGNAL_H
#define PQNET_UTIL_SIGNAL_H

#include <csignal>

#include "types.h"

namespace pqnet
{

void waitSig();
// Add or Update signal(s)
void addSignal(int signum, pn_signal_func func);
// Delete signal(s)
void delSignal(int signum);

} // namespace pqnet

#endif // PQNET_UTIL_SIGNAL_H
