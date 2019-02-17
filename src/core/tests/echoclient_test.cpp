#include "../client.h"

int main()
{
    pqnet::TcpEchoClient echocli;
    echocli.run();
    return 0;
}
