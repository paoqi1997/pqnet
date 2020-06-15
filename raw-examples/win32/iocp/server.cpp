#include <cstdio>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>

#include <winsock2.h>

enum io_operation {
    IO_READ,
    IO_WRITE
};

struct io_context
{
    OVERLAPPED overlapped;
    SOCKET sockfd;
    WSABUF wsabuf;
    char buf[1024];
    io_operation opType;
};

std::size_t getNumberOfProcessors();
void routine(HANDLE handle);

int main()
{
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    SOCKET listenfd = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
    if (listenfd == INVALID_SOCKET) {
        printf("WSASocket failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    sockaddr_in servaddr;
    std::memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(12488);
    servaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    if (bind(listenfd, reinterpret_cast<sockaddr*>(&servaddr), sizeof(servaddr)) == -1) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        closesocket(listenfd);
        WSACleanup();
        return 1;
    }

    if (listen(listenfd, SOMAXCONN) == -1) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(listenfd);
        WSACleanup();
        return 1;
    }

    std::size_t nProcessors = getNumberOfProcessors();
    std::size_t nThreads = nProcessors * 2;
    HANDLE hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, DWORD(nThreads));

    std::vector<std::thread> threads;
    for (std::size_t i = 0; i < nThreads; ++i) {
        threads.emplace_back(routine, hIOCP);
    }

    for (;;) {
        SOCKET connfd = accept(listenfd, nullptr, nullptr);

        printf("Connection %d coming...\n", int(connfd));

        if (CreateIoCompletionPort(reinterpret_cast<HANDLE>(connfd), hIOCP, 0, 0) == nullptr) {
            printf("CreateIoCompletionPort failed with error: %d\n", WSAGetLastError());
            closesocket(connfd);
            break;
        } else {
            auto io_ctx = new io_context;
            std::memset(io_ctx->buf, 0, sizeof(io_ctx->buf));
            std::memset(&io_ctx->overlapped, 0, sizeof(io_ctx->overlapped));
            DWORD recvBytes = sizeof(io_ctx->buf);
            io_ctx->sockfd = connfd;
            io_ctx->wsabuf.buf = io_ctx->buf;
            io_ctx->wsabuf.len = recvBytes;
            io_ctx->opType = IO_READ;

            DWORD flags = 0;
            int iResult = WSARecv(
                connfd, &io_ctx->wsabuf, 1, &recvBytes, &flags, &io_ctx->overlapped, nullptr
            );
            if (iResult == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING) {
                printf("WSARecv failed with error: %d\n", WSAGetLastError());
                closesocket(connfd);
                delete io_ctx;
                break;
            } else {
                printf("MainRecv: %s\n", io_ctx->buf);
            }
        }
    }

    closesocket(listenfd);
    WSACleanup();

    return 0;
}

std::size_t getNumberOfProcessors()
{
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
}

void routine(HANDLE handle)
{
    HANDLE hIOCP = handle;
    DWORD ioSize = 0;
    void *lpCompletionKey = nullptr;
    LPOVERLAPPED lpoverlapped = nullptr;

    for (;;) {
        BOOL succ = GetQueuedCompletionStatus(
            hIOCP, &ioSize, reinterpret_cast<PULONG_PTR>(&lpCompletionKey), &lpoverlapped, INFINITE
        );

        auto io_ctx = reinterpret_cast<io_context*>(lpoverlapped);

        if (ioSize == 0) {
            printf("Connection %d closing...\n", int(io_ctx->sockfd));
            closesocket(io_ctx->sockfd);
            delete io_ctx;
            continue;
        }
        if (io_ctx->opType == IO_READ) {
            std::memset(&io_ctx->overlapped, 0, sizeof(io_ctx->overlapped));
            DWORD readBytes = DWORD(std::strlen(io_ctx->buf) + 1);
            io_ctx->wsabuf.buf = io_ctx->buf;
            io_ctx->wsabuf.len = readBytes;
            io_ctx->opType = IO_WRITE;

            DWORD flags = 0;
            int iResult = WSASend(
                io_ctx->sockfd, &io_ctx->wsabuf, 1, &readBytes, flags, &io_ctx->overlapped, nullptr
            );
            if (iResult == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING) {
                printf("WSASend failed with error: %d\n", WSAGetLastError());
                closesocket(io_ctx->sockfd);
                delete io_ctx;
                continue;
            }

            printf("Send: %s\n", io_ctx->buf);
            std::memset(io_ctx->buf, 0, sizeof(io_ctx->buf));
        }
        else if (io_ctx->opType == IO_WRITE) {
            std::memset(&io_ctx->overlapped, 0, sizeof(io_ctx->overlapped));
            DWORD recvBytes = sizeof(io_ctx->buf);
            io_ctx->wsabuf.buf = io_ctx->buf;
            io_ctx->wsabuf.len = recvBytes;
            io_ctx->opType = IO_READ;

            DWORD flags = 0;
            int iResult = WSARecv(
                io_ctx->sockfd, &io_ctx->wsabuf, 1, &recvBytes, &flags, &io_ctx->overlapped, nullptr
            );
            if (iResult == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING) {
                printf("WSARecv failed with error: %d\n", WSAGetLastError());
                closesocket(io_ctx->sockfd);
                delete io_ctx;
                continue;
            }

            printf("Recv: %s\n", io_ctx->buf);
        }
    }
}
