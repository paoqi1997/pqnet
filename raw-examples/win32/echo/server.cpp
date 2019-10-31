#include <iostream>

#include <winsock2.h>

int main()
{
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::printf("WSAStartup failed with error: %d\n", iResult);
    }

    WSACleanup();

    return 0;
}
