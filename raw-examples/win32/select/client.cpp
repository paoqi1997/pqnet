#include <iostream>

#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

const char *IPADDR = "127.0.0.1";
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
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    iResult = getaddrinfo(IPADDR, PORT, &hints, &res);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    SOCKET sockfd;
    for (auto p = res; p != nullptr; p = p->ai_next) {
        // socket
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // connect
        iResult = connect(sockfd, p->ai_addr, static_cast<int>(p->ai_addrlen));
        if (iResult == SOCKET_ERROR) {
            closesocket(sockfd);
            sockfd = INVALID_SOCKET;
            continue;
        }

        break;
    }

    freeaddrinfo(res);
    if (sockfd == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    char buf[1024];
    for (;;) {
        cin >> buf;
        // send
        iResult = send(sockfd, buf, static_cast<int>(strlen(buf)), 0);
        if (iResult == SOCKET_ERROR) {
            printf("send failed with error: %d\n", WSAGetLastError());
            break;
        }
        // recv
        iResult = recv(sockfd, buf, sizeof(buf), 0);
        if (iResult > 0) {
            printf("Recv: %s\n", buf);
        } else if (iResult == 0) {
            printf("Connection closed\n");
            break;
        } else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            break;
        }
    }

    closesocket(sockfd);
    WSACleanup();

    return 0;
}
