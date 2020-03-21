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
        printf("WSAStartup failed with error: %d\n", iResult);
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
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // socket
    SOCKET listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (listenfd == INVALID_SOCKET) {
        printf("socket failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(res);
        WSACleanup();
        return 1;
    }

    // bind
    iResult = bind(listenfd, res->ai_addr, static_cast<int>(res->ai_addrlen));
    freeaddrinfo(res);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        closesocket(listenfd);
        WSACleanup();
        return 1;
    }

    // listen
    iResult = listen(listenfd, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(listenfd);
        WSACleanup();
        return 1;
    }

    fd_set mySet;
    FD_ZERO(&mySet);
    FD_SET(listenfd, &mySet);

    char buf[1024] = {0};
    for (;;) {
        fd_set tmpSet = mySet;
        // select
        int cnt = select(0, &tmpSet, nullptr, nullptr, nullptr);
        if (cnt == SOCKET_ERROR) {
            printf("select failed with error: %d\n", WSAGetLastError());
            break;
        }
        for (std::size_t i = 0; i < mySet.fd_count; ++i) {
            SOCKET sockfd = mySet.fd_array[i];
            if (!FD_ISSET(sockfd, &tmpSet)) {
                continue;
            }
            /// Listenfd
            if (sockfd == listenfd) {
                SOCKET connfd = accept(listenfd, nullptr, nullptr);
                if (connfd == INVALID_SOCKET) {
                    printf("accept failed with error: %d\n", WSAGetLastError());
                    break;
                }
                if (mySet.fd_count == FD_SETSIZE) {
                    printf("too many clients!\n");
                    continue;
                }
                FD_SET(connfd, &mySet);
                printf("Connection %d coming...\n", connfd);
                continue;
            }
            /// Clients
            SOCKET connfd = sockfd;
            // recv
            iResult = recv(connfd, buf, sizeof(buf), 0);
            if (iResult > 0) {
                printf("Recv: %s\n", buf);
                // send
                int iSendResult = send(connfd, buf, iResult, 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("send failed with error: %d\n", WSAGetLastError());
                    closesocket(connfd);
                    break;
                }
            } else if (iResult == -1 && WSAGetLastError() == WSAECONNRESET) {
                printf("Connection %d closing...\n", connfd);
                closesocket(connfd);
                FD_CLR(connfd, &mySet);
                continue;
            } else {
                printf("recv failed with error: %d\n", WSAGetLastError());
                closesocket(connfd);
                break;
            }
        }
    }

    closesocket(listenfd);
    WSACleanup();

    return 0;
}
