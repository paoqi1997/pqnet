#ifndef PQNET_UTIL_TYPES_H
#define PQNET_UTIL_TYPES_H

#include <functional>

namespace pqnet
{

// Event Codes
#define EV_CONN 0x01
#define EV_EXIT 0x02

// used by listen(2)
#define BACKLOG 16
// used by TimeStamp
#define BUFSIZE 32

// Messages
#define MSG_EPOLL_WAIT_EINTR "The Loop was interrupted by a signal handler."

// Constants
#define    TRUE  1
#define   FALSE  0
#define UNKNOWN -1

#define K1E3 (1000)
#define K1E6 (1000 * 1000)
#define K1E9 (1000 * 1000 * 1000)

using pn_thread_func_t = void*(*)(void*);

using pn_task_func_t = void(*)(void*);

using pn_signal_func_t = std::function<void()>;

} // namespace pqnet

#endif // PQNET_UTIL_TYPES_H
