#include "../logger.h"

int main()
{
    do {
        const char *name = "pqnet";
        INFO("Hello %s!", name);
        LOG_INFO("Hello %s!", name);
    } while (false);
    return 0;
}
