#include <cstring>
#include <iostream>

#ifndef FD_SETSIZE
#define FD_SETSIZE 1024
#endif // FD_SETSIZE

#include <winsock2.h>
#include <ws2tcpip.h>

const char *PORT = "12488";

int main()
{
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    struct addrinfo hints, *res = nullptr;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(nullptr, PORT, &hints, &res);
    if (iResult != 0) {
        std::printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // socket
    SOCKET listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (listenfd == INVALID_SOCKET) {
        std::printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(res);
        WSACleanup();
        return 1;
    }

    // bind
    iResult = bind(listenfd, res->ai_addr, static_cast<int>(res->ai_addrlen));
    freeaddrinfo(res);
    if (iResult == SOCKET_ERROR) {
        std::printf("bind failed with error: %d\n", WSAGetLastError());
        closesocket(listenfd);
        WSACleanup();
        return 1;
    }

    // listen
    iResult = listen(listenfd, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        std::printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(listenfd);
        WSACleanup();
        return 1;
    }

    fd_set mySet;
    FD_ZERO(&mySet);
    FD_SET(listenfd, &mySet);

    char recvBuf[1024] = {0};
    for (;;) {
        fd_set tmpSet = mySet;
        // select
        int cnt = select(0, &tmpSet, nullptr, nullptr, nullptr);
        if (cnt == SOCKET_ERROR) {
            std::printf("select failed with error: %d\n", WSAGetLastError());
            break;
        }
        // For ListenFd
        if (FD_ISSET(listenfd, &tmpSet)) {
            SOCKET connfd = accept(listenfd, nullptr, nullptr);
            if (connfd == INVALID_SOCKET) {
                std::printf("accept failed with error: %d\n", WSAGetLastError());
                break;
            }
            if (mySet.fd_count == FD_SETSIZE) {
                std::printf("too many clients!\n");
                continue;
            }
            FD_SET(connfd, &mySet);
        } else {
            // For Clients
            for (std::size_t i = 0; i < mySet.fd_count; ++i) {
                SOCKET connfd = mySet.fd_array[i];
                if (!FD_ISSET(connfd, &tmpSet)) {
                    continue;
                }
                // recv
                iResult = recv(connfd, recvBuf, sizeof(recvBuf), 0);
                if (iResult > 0) {
                    std::printf("Recv: %s\n", recvBuf);
                    // send
                    int iSendResult = send(connfd, recvBuf, iResult, 0);
                    if (iSendResult == SOCKET_ERROR) {
                        std::printf("send failed with error: %d\n", WSAGetLastError());
                        closesocket(connfd);
                        WSACleanup();
                        return 1;
                    }
                } else if (iResult == 0 || WSAGetLastError() == WSAECONNRESET) {
                    std::printf("Connection closing...\n");
                    closesocket(connfd);
                    FD_CLR(connfd, &mySet);
                    continue;
                } else {
                    std::printf("recv failed with error: %d\n", WSAGetLastError());
                    closesocket(connfd);
                    WSACleanup();
                    return 1;
                }
                std::memset(recvBuf, 0, sizeof(recvBuf));
            }
        }
    }

    closesocket(listenfd);
    WSACleanup();

    return 0;
}
