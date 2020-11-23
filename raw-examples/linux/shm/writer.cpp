#include "util.h"

int main()
{
    int shmid = shmget(SHM_KEY, SHM_SIZE, SHM_R | SHM_W | IPC_CREAT);
    if (shmid == -1) {
        error("shmget");
        return 1;
    }

    int semid = semget(SEM_KEY, 2, IPC_CREAT | 0600);
    if (semid == -1) {
        error("semget");
        return 1;
    }

    auto shmp = shmat(shmid, nullptr, 0);
    if (shmp == reinterpret_cast<void*>(-1)) {
        error("shmat");
        return 1;
    }

    union semun sem_arg;
    unsigned short vals[2] = {0, 1};
    sem_arg.array = vals;

    // SETVAL: 设置成员 semnum 的 semval 值，该值由 arg.val 指定
    // SETALL: 将该集合中所有的信号量值设置成 arg.array 指向的数组中的值
    if (semctl(semid, SEM_XYZ, SETALL, sem_arg) == -1) {
        error("semctl");
        return 1;
    }

    struct Msg *msg = static_cast<struct Msg*>(shmp);
    for (std::size_t i = 0; i <= 10; ++i) {
        if (sem_p2(semid) == -1) {
            error("sem_p2");
            return 1;
        }
        msg->num = i;
        std::sprintf(msg->buf, "shm: %zu", i);
        std::printf("[sendmsg] %s\n", msg->buf);
        if (sem_v1(semid) == -1) {
            error("sem_v1");
            return 1;
        }
        sleep(1);
    }

    if (shmdt(shmp) == -1 && errno != EINVAL) {
        error("shmdt");
        return 1;
    }

    if (shmctl(shmid, IPC_RMID, nullptr) == -1 && errno != EINVAL) {
        error("shmctl");
        return 1;
    }

    if (semctl(semid, SEM_XYZ, IPC_RMID, nullptr) == -1 && errno != EINVAL) {
        error("semctl");
        return 1;
    }

    return 0;
}
