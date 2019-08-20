#ifndef PQNET_UTIL_TYPES_H
#define PQNET_UTIL_TYPES_H

#include <functional>

namespace pqnet
{

#define   EV_CONN    1
#define   EV_EXIT    2

#define   BACKLOG   16
#define   BUFSIZE   32
#define EPOLLSIZE 1024

using pn_thread_func_t = void*(*)(void*);

using pn_task_func_t = void(*)(void*);

using pn_signal_func_t = std::function<void()>;

} // namespace pqnet

#endif // PQNET_UTIL_TYPES_H