#ifndef PQNET_UTIL_SIGNAL_H
#define PQNET_UTIL_SIGNAL_H

#include <csignal>
#include <functional>

namespace pqnet
{

using signalCallBack = std::function<void()>;

void waitSig();
// Add or Update signal(s)
void addSignal(int signum, const signalCallBack& func);
// Delete signal(s)
void delSignal(int signum);

} // namespace pqnet

#endif // PQNET_UTIL_SIGNAL_H
