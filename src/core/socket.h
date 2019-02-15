#ifndef PQNET_CORE_SOCKET_H
#define PQNET_CORE_SOCKET_H

namespace pqnet
{

int new_socket();
bool setNonBlock(int sockfd, bool flag);
bool setReuseAddr(int sockfd, bool flag);
bool setReusePort(int sockfd, bool flag);

} // namespace pqnet

#endif // PQNET_CORE_SOCKET_H
