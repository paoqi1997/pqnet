#include <iostream>

#include "../../util/signal.h"
#include "../client.h"

void sighandler(int signo) {
    if (signo == SIGINT) {
        std::cout << std::endl;
    }
    std::cout << "Exit echo client." << std::endl;
}

int main()
{
    pqnet::TcpEchoClient echocli("127.0.0.1", 12488);
    pqnet::Signal sig;
    sig.addSignal(SIGINT, sighandler);
    sig.addSignal(SIGTERM, sighandler);
    sig.waitSig();
    echocli.run();
    return 0;
}
