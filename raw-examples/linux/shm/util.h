#ifndef UTIL_H
#define UTIL_H

#include <cerrno>
#include <cstdio>
#include <cstring>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

struct Msg
{
    int num;
    char buf[32];
};

#define SHM_KEY  0x48
#define SHM_SIZE (sizeof(struct Msg))

#define SEM_KEY  0x64
#define SEM_XYZ  2020

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

/**
 * struct sembuf {
 *     unsigned short sem_num;
 *     short          sem_op;
 *     short          sem_flg;
 * };
 * 1) 若sem_op为正值，这对应于进程释放的占用的资源数
 * 2) 若sem_op为负值，表示要获取由该信号量控制的资源
 * 3) sem_num用于指定信号量集合中的一个成员，这里0代表writer，1代表reader
 */
struct sembuf sem_opt_wait1 = { 0, -1, SEM_UNDO };
struct sembuf sem_opt_wait2 = { 1, -1, SEM_UNDO };
struct sembuf sem_opt_post1 = { 0,  1, SEM_UNDO };
struct sembuf sem_opt_post2 = { 1,  1, SEM_UNDO };

/**
 * 1 0: 进程1准备写，进程2未启动，没有读
 * 0 0: 进程1写
 * 0 1: 进程1写完，进程2启动，准备读
 * 0 0: 进程2读
 * 1 0: 进程2读完，进程1变为可写状态
 * ...
 */

inline void error(const char *callname)
{
    std::printf(
        "Failed to call %s(), error: %s\n", callname, std::strerror(errno)
    );
}

inline int sem_p1(int semid)
{
    return semop(semid, &sem_opt_wait1, 1);
}

inline int sem_p2(int semid)
{
    return semop(semid, &sem_opt_wait2, 1);
}

inline int sem_v1(int semid)
{
    return semop(semid, &sem_opt_post1, 1);
}

inline int sem_v2(int semid)
{
    return semop(semid, &sem_opt_post2, 1);
}

#endif // UTIL_H
