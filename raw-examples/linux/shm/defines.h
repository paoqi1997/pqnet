#ifndef UTIL_H
#define UTIL_H

struct Msg
{
    int num;
    char buf[32];
};

#define SHM_KEY  0x48 // 0x64不可
#define SHM_SIZE (sizeof(struct Msg))

#endif // UTIL_H
