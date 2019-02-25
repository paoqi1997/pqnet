#include <iostream>

#include "../../util/logger.h"
#include "../../util/signal.h"
#include "../server.h"

using namespace std::placeholders;

class TcpEchoServer
{
public:
    TcpEchoServer(std::uint16_t port) : serv(port) {}
    void run() {
        serv.setConnectCallBack(
            std::bind(&TcpEchoServer::onConnect, this, _1)
        );
        serv.setReadCallBack(
            std::bind(&TcpEchoServer::onRead, this, _1)
        );
        serv.setMessageCallBack(
            std::bind(&TcpEchoServer::onMessage, this, _1)
        );
        serv.setCloseByPeerCallBack(
            std::bind(&TcpEchoServer::onCloseByPeer, this, _1)
        );
        serv.setCloseBySockCallBack(
            std::bind(&TcpEchoServer::onCloseBySock, this, _1)
        );
        serv.run();
    }
    void preShutdown() {
        serv.preShutdown();
    }
    void onConnect(const pqnet::TcpConnPtr& conn) {
        conn->send("Hello!\n");
        TRACE("%d connect.", conn->getFd());
    }
    void onRead(const pqnet::TcpConnPtr& conn) {
        conn->recv();
        TRACE("%d read.", conn->getFd());
    }
    void onMessage(const pqnet::TcpConnPtr& conn) {
        conn->send();
        TRACE("%d message.", conn->getFd());
    }
    void onCloseByPeer(const pqnet::TcpConnPtr& conn) {
        TRACE("%d closed by itself.", conn->getFd());
    }
    void onCloseBySock(const pqnet::TcpConnPtr& conn) {
        conn->send("quit\n");
        TRACE("%d closed by server.", conn->getFd());
    }
private:
    pqnet::TcpServer serv;
};

int main()
{
    TcpEchoServer echoserv(12488);
    auto SIGINT_HANDLER = [&](){
        echoserv.preShutdown();
        std::cout << std::endl;
        std::cout << "Exit echo server." << std::endl;
    };
    auto SIGTERM_HANDLER = [&](){
        echoserv.preShutdown();
        std::cout << "Exit echo server." << std::endl;
    };
    pqnet::addSignal(SIGINT, SIGINT_HANDLER);
    pqnet::addSignal(SIGTERM, SIGTERM_HANDLER);
    pqnet::waitSig();
    echoserv.run();
    return 0;
}
