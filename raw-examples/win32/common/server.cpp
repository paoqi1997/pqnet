#include <iostream>

#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

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
        printf("socket failed with error: %ld\n", WSAGetLastError());
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

    SOCKET connfd = INVALID_SOCKET;

    char buf[1024] = {0};
    for (;;) {
        if (connfd == INVALID_SOCKET) {
            // accept
            connfd = accept(listenfd, nullptr, nullptr);
            if (connfd == INVALID_SOCKET) {
                printf("accept failed with error: %d\n", WSAGetLastError());
                WSACleanup();
                return 1;
            }
            printf("Connection %d coming...\n", connfd);
        }
        // recv
        iResult = recv(connfd, buf, sizeof(buf), 0);
        if (iResult > 0) {
            printf("Recv: %s\n", buf);
            // send
            int iSendResult = send(connfd, buf, iResult, 0);
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                break;
            }
        } else if (iResult == -1 && WSAGetLastError() == WSAECONNRESET) {
            printf("Connection %d closing...\n", connfd);
            closesocket(connfd);
            connfd = INVALID_SOCKET;
            continue;
        } else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            break;
        }
    }

    closesocket(connfd);
    closesocket(listenfd);
    WSACleanup();

    return 0;
}
