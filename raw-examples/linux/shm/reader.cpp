#include <cerrno>
#include <cstdio>
#include <cstring>
#include <iostream>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#include "defines.h"

using std::cout;
using std::endl;

int main()
{
    int shmid = shmget(SHM_KEY, SHM_SIZE, SHM_R | SHM_W | IPC_CREAT);
    if (shmid == -1) {
        cout << std::strerror(errno) << endl;
        return 1;
    }

    auto shmp = shmat(shmid, nullptr, 0);
    if (shmp == reinterpret_cast<void*>(-1)) {
        cout << std::strerror(errno) << endl;
        return 1;
    }

    struct Msg *msg = static_cast<struct Msg*>(shmp);
    while (msg->num < 10) {
        std::printf("%s\n", msg->buf);
        sleep(1);
    }

    if (shmdt(shmp) == -1) {
        cout << std::strerror(errno) << endl;
        return 1;
    }

    if (shmctl(shmid, IPC_RMID, nullptr) == -1) {
        cout << std::strerror(errno) << endl;
        return 1;
    }

    return 0;
}
