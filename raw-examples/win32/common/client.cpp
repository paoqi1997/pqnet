#include <cstring>
#include <iostream>

#include <winsock2.h>
#include <ws2tcpip.h>

const char *IPADDR = "127.0.0.1";
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
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    iResult = getaddrinfo(IPADDR, PORT, &hints, &res);
    if (iResult != 0) {
        std::printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // socket
    SOCKET sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == INVALID_SOCKET) {
        std::printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(res);
        WSACleanup();
        return 1;
    }

    // connect
    iResult = connect(sockfd, res->ai_addr, static_cast<int>(res->ai_addrlen));
    freeaddrinfo(res);
    if (iResult == SOCKET_ERROR) {
        std::printf("Unable to connect to server!\n");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    char sendBuf[1024], recvBuf[1024] = {0};
    for (;;) {
        std::cin >> sendBuf;
        // send
        iResult = send(sockfd, sendBuf, static_cast<int>(std::strlen(sendBuf)), 0);
        if (iResult == SOCKET_ERROR) {
            std::printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(sockfd);
            WSACleanup();
            return 1;
        }
        // recv
        iResult = recv(sockfd, recvBuf, sizeof(recvBuf), 0);
        if (iResult > 0) {
            std::printf("Recv: %s\n", recvBuf);
        } else if (iResult == 0) {
            std::printf("Connection closed\n");
            break;
        } else {
            std::printf("recv failed with error: %d\n", WSAGetLastError());
        }
        std::memset(recvBuf, 0, sizeof(recvBuf));
    }

    closesocket(sockfd);
    WSACleanup();

    return 0;
}
