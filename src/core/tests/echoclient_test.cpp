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
    pqnet::Signal sig(sighandler);
    sig.addSignal(SIGINT);
    sig.addSignal(SIGTERM);
    sig.waitSig();
    echocli.run();
    return 0;
}
