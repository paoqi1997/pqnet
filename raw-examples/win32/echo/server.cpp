#include <iostream>

#include <winsock2.h>
#include <ws2tcpip.h>

int main()
{
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    struct addrinfo hints, *result = nullptr;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(nullptr, "12488", &hints, &result);
    if (iResult != 0) {
        std::printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // socket
    SOCKET listenfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenfd == INVALID_SOCKET) {
        std::printf("socket failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // bind
    iResult = bind(listenfd, result->ai_addr, static_cast<int>(result->ai_addrlen));
    if (iResult == SOCKET_ERROR) {
        std::printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(listenfd);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    // listen
    iResult = listen(listenfd, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        std::printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(listenfd);
        WSACleanup();
        return 1;
    }

    // accept
    SOCKET connfd = accept(listenfd, nullptr, nullptr);
    if (connfd == INVALID_SOCKET) {
        std::printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(listenfd);
        WSACleanup();
        return 1;
    }

    closesocket(listenfd);

    char recvBuf[1024];
    do {
        iResult = recv(connfd, recvBuf, 1024, 0);
        if (iResult > 0) {
            std::printf("Recv: %s\n", recvBuf);
            int iSendResult = send(connfd, recvBuf, iResult, 0);
            if (iSendResult == SOCKET_ERROR) {
                std::printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(connfd);
                WSACleanup();
                return 1;
            }
        } else if (iResult == 0) {
            std::printf("Connection closing...\n");
        } else {
            std::printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(connfd);
            WSACleanup();
            return 1;
        }
    } while (iResult > 0);

    iResult = shutdown(connfd, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        std::printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(connfd);
        WSACleanup();
        return 1;
    }

    closesocket(connfd);
    WSACleanup();

    return 0;
}
