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
    for (std::size_t i = 0; i <= 10; ++i) {
        msg->num = i;
        std::sprintf(msg->buf, "shm: %zu", i);
        sleep(1);
    }

    if (shmdt(shmp) == -1 && errno != EINVAL) {
        cout << std::strerror(errno) << endl;
        return 1;
    }

    if (shmctl(shmid, IPC_RMID, nullptr) == -1 && errno != EINVAL) {
        cout << std::strerror(errno) << endl;
        return 1;
    }

    return 0;
}
