#include <iostream>

#include "../../util/signal.h"
#include "../client.h"

auto SIGINT_HANDLER = [](){
    std::cout << std::endl;
    std::cout << "Exit echo client." << std::endl;
};

auto SIGTERM_HANDLER = [](){
    std::cout << "Exit echo client." << std::endl;
};

int main()
{
    pqnet::TcpEchoClient echocli("127.0.0.1", 12488);
    pqnet::addSignal(SIGINT, SIGINT_HANDLER);
    pqnet::addSignal(SIGTERM， SIGTERM_HANDLER);
    pqnet::waitSig();
    echocli.run();
    return 0;
}
