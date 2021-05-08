#ifndef PQNET_UTIL_DEFINES_H
#define PQNET_UTIL_DEFINES_H

namespace pqnet
{

// used by listen(2)
#define BACKLOG 16
// used by TimeStamp
#define BUFSIZE 32

// Messages
#define MSG_ACCEPT_EAGAIN       "The socket is marked nonblocking and no connections are present to be accepted."
#define MSG_EPOLL_WAIT_EINTR    "The call was interrupted by a signal handler."
#define MSG_BUFFER_SIZE_MAXIMUM "The size of the buffer is maximum."

// Constants
#define    TRUE  1
#define   FALSE  0
#define UNKNOWN -1

#define L_ENDIAN 1
#define B_ENDIAN 2
#define U_ENDIAN 3

#define K1E3 (1000)
#define K1E6 (1000 * 1000)
#define K1E9 (1000 * 1000 * 1000)

// HTTP
#define   LF "\n"
#define CRLF "\r\n"

#define HTTP_VERSION_1_0 "HTTP/1.0"
#define HTTP_VERSION_1_1 "HTTP/1.1"
#define HTTP_VERSION_2_0 "HTTP/2.0"

#define HTTP_STATUS_CODE_200 "200 OK"
#define HTTP_STATUS_CODE_404 "404 Not Found"

} // namespace pqnet

#endif // PQNET_UTIL_DEFINES_H
