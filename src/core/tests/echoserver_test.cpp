#include <iostream>

#include "../../util/signal.h"
#include "../callback.h"
#include "../server.h"

void sighandler(int signo) {
    if (signo == SIGINT) {
        std::cout << std::endl;
    }
    std::cout << "Exit echo server." << std::endl;
}

void onConnect(const pqnet::TcpConnPtr& conn) { conn->send("Hello!\n"); }
void onClose(const pqnet::TcpConnPtr& conn) { conn->send("Bye!\n"); }
void onRead(const pqnet::TcpConnPtr& conn) { conn->recv(); }
void onMessage(const pqnet::TcpConnPtr& conn) { conn->send(); }

int main()
{
    pqnet::TcpServer serv(12488);
    serv.setConnectCallBack(onConnect);
    serv.setCloseCallBack(onClose);
    serv.setReadCallBack(onRead);
    serv.setMessageCallBack(onMessage);
    pqnet::Signal sig(sighandler);
    sig.addSignal(SIGINT);
    sig.addSignal(SIGTERM);
    sig.waitSig();
    serv.run();
    return 0;
}
