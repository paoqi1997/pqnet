#include <iostream>

#include "../../util/logger.h"
#include "../../util/signal.h"
#include "../server.h"

using namespace std::placeholders;

auto SIGINT_HANDLER = [](){
    std::cout << std::endl;
    std::cout << "Exit echo server." << std::endl;
};

auto SIGTERM_HANDLER = [](){
    std::cout << "Exit echo server." << std::endl;
};

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
        TRACE("%d connect.", conn->getFd());
    }
    void onClose(const pqnet::TcpConnPtr& conn) {
        conn->send("quit\n");
        TRACE("%d close.", conn->getFd());
    }
    void onRead(const pqnet::TcpConnPtr& conn) {
        conn->recv();
        TRACE("%d read.", conn->getFd());
    }
    void onMessage(const pqnet::TcpConnPtr& conn) {
        conn->send();
        TRACE("%d message.", conn->getFd());
    }
private:
    pqnet::TcpServer serv;
};

int main()
{
    TcpEchoServer echoserv(12488);
    pqnet::addSignal(SIGINT, SIGINT_HANDLER);
    pqnet::addSignal(SIGTERM, SIGTERM_HANDLER);
    pqnet::waitSig();
    echoserv.run();
    return 0;
}
