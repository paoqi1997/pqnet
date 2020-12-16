#include <cassert>
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include <sys/resource.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#define K8MB (8 * 1024 * 1024)

using std::printf;

int main()
{
    pid_t tid = syscall(SYS_gettid);
    pid_t pid = getpid();

    printf("ThreadID: %d, ProcessID: %d\n", tid, pid);

    struct rlimit rlim;
    if (getrlimit(RLIMIT_STACK, &rlim) == -1) {
        printf("Failed to call getrlimit(), error: %s\n", std::strerror(errno));
    }

    assert(rlim.rlim_cur == K8MB);
    assert(rlim.rlim_max == UINT64_MAX);

    return 0;
}
