#ifndef PQNET_UTIL_TYPES_H
#define PQNET_UTIL_TYPES_H

namespace pqnet
{

#define BACKLOG   16
#define BUFSIZE   32
#define MAXEVENTS 2048

using pn_thread_func = void*(*)(void*);

using pn_task_func = pn_thread_func;
using pn_task_arg  = void*;

using pn_signal_func = void(*)(int);

} // namespace pqnet

#endif // PQNET_UTIL_TYPES_H
