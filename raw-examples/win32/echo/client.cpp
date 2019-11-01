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

    WSACleanup();

    return 0;
}
