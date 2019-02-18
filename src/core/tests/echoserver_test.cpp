#include <iostream>

#include "../../util/signal.h"
#include "../server.h"

using namespace std::placeholders;

void sighandler(int signo) {
    if (signo == SIGINT) {
        std::cout << std::endl;
    }
    std::cout << "Exit echo server." << std::endl;
}

class TcpEchoServer
{
public:
    TcpEchoServer(std::uint16_t port) : serv(port) {}
    void run() {
        serv.setConnectCallBack(
            std::bind(&TcpEchoServer::onConnect, this, _1)
        );
        serv.setCloseCallBack(
            std::bind(&TcpEchoServer::onClose, this, _1)
        );
        serv.setReadCallBack(
            std::bind(&TcpEchoServer::onRead, this, _1)
        );
        serv.setMessageCallBack(
            std::bind(&TcpEchoServer::onMessage, this, _1)
        );
        serv.run();
    }
    void onConnect(const pqnet::TcpConnPtr& conn) {
        conn->send("Hello!\n");
    }
    void onClose(const pqnet::TcpConnPtr& conn) {
        conn->send("Bye!\n");
    }
    void onRead(const pqnet::TcpConnPtr& conn) {
        conn->recv();
    }
    void onMessage(const pqnet::TcpConnPtr& conn) {
        conn->send();
    }
private:
    pqnet::TcpServer serv;
};

int main()
{
    TcpEchoServer echoserv(12488);
    pqnet::Signal sig(sighandler);
    sig.addSignal(SIGINT);
    sig.addSignal(SIGTERM);
    sig.waitSig();
    echoserv.run();
    return 0;
}
