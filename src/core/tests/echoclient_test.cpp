#include <iostream>

#include "../../util/signal.h"
#include "../client.h"

int main()
{
    pqnet::TcpEchoClient echocli("127.0.0.1", 12488);
    auto SIGINT_HANDLER = [&](){
        echocli.shutdown();
        std::cout << std::endl;
        std::cout << "Exit echo client." << std::endl;
    };
    auto SIGTERM_HANDLER = [&](){
        echocli.shutdown();
        std::cout << "Exit echo client." << std::endl;
    };
    pqnet::addSignal(SIGINT, SIGINT_HANDLER);
    pqnet::addSignal(SIGTERM, SIGTERM_HANDLER);
    pqnet::waitSig();
    echocli.run();
    return 0;
}
