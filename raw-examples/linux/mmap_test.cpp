#include <algorithm>
#include <cerrno>
#include <cstring>
#include <iostream>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>

#define K1MB (1024 * 1024)

using std::cout;
using std::endl;

int main()
{
    int infd = open("./main.sh", O_RDONLY);
    if (infd == -1) {
        cout << std::strerror(errno) << endl;
        return 1;
    }

    int outfd = open("./main.sh.bak", O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (outfd == -1) {
        cout << std::strerror(errno) << endl;
        return 1;
    }

    struct stat statbuf;
    if (fstat(infd, &statbuf) == -1) {
        cout << std::strerror(errno) << endl;
        return 1;
    }

    off_t filesize = statbuf.st_size;

    if (ftruncate(outfd, filesize) == -1) {
        cout << std::strerror(errno) << endl;
        return 1;
    }

    off_t pos = 0;
    std::size_t copysize = 0;

    while (pos < filesize) {
        copysize = std::min(static_cast<int>(filesize - pos), K1MB);

        auto src = mmap(0, copysize, PROT_READ, MAP_SHARED, infd, pos);
        if (src == MAP_FAILED) {
            cout << std::strerror(errno) << endl;
            return 1;
        }

        auto dest = mmap(0, copysize, PROT_READ | PROT_WRITE, MAP_SHARED, outfd, pos);
        if (dest == MAP_FAILED) {
            cout << std::strerror(errno) << endl;
            return 1;
        }

        std::memcpy(dest, src, copysize);
        munmap(src, copysize);
        munmap(dest, copysize);

        pos += copysize;
    }

    return 0;
}
