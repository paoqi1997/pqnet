#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#include "util.h"

int main()
{
    int shmid = shmget(SHM_KEY, SHM_SIZE, SHM_R | SHM_W | IPC_CREAT);
    if (shmid == -1) {
        error("shmid");
        return 1;
    }

    int semid = semget(SEM_KEY, 0, IPC_CREAT | 0666);
    if (semid == -1) {
        error("semget");
        return 1;
    }

    auto shmp = shmat(shmid, nullptr, 0);
    if (shmp == reinterpret_cast<void*>(-1)) {
        error("shmat");
        return 1;
    }

    struct Msg *msg = static_cast<struct Msg*>(shmp);
    while (msg->num < 10) {
        if (semop(semid, &sem_opt_wait1, 1) == -1) {
            error("semop");
            return 1;
        }
        std::printf("[recvmsg] %s\n", msg->buf);
        if (semop(semid, &sem_opt_wakeup2, 1) == -1) {
            error("semop");
            return 1;
        }
        sleep(1);
    }

    if (shmdt(shmp) == -1) {
        error("shmdt");
        return 1;
    }

    if (shmctl(shmid, IPC_RMID, nullptr) == -1) {
        error("shmctl");
        return 1;
    }

    return 0;
}
