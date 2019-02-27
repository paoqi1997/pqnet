#include <iostream>

#include "../../util/logger.h"
#include "../../util/signal.h"
#include "../client.h"

using namespace std::placeholders;

class TcpEchoClient
{
public:
    TcpEchoClient(const char *servname, std::uint16_t port)
        : cli(servname, port) {}
    void run() {
        cli.setInCallBack(
            std::bind(&TcpEchoClient::handleIn, this, _1)
        );
        cli.setConnectCallBack(
            std::bind(&TcpEchoClient::onConnect, this, _1)
        );
        cli.setReadCallBack(
            std::bind(&TcpEchoClient::onRead, this, _1)
        );
        cli.setMessageCallBack(
            std::bind(&TcpEchoClient::onMessage, this, _1)
        );
        cli.setCloseByPeerCallBack(
            std::bind(&TcpEchoClient::onCloseByPeer, this, _1)
        );
        cli.setCloseBySockCallBack(
            std::bind(&TcpEchoClient::onCloseBySock, this, _1)
        );
        cli.run();
    }
    void shutdown() {
        cli.shutdown();
    }
    void handleIn(const pqnet::TcpConnPtr& conn) {
        std::cin >> msg; msg += '\n';
        conn->append(msg.c_str());
        conn->send();
    }
    void onConnect(const pqnet::TcpConnPtr& conn) {
        // None
    }
    void onRead(const pqnet::TcpConnPtr& conn) {
        conn->recv();
    }
    void onMessage(const pqnet::TcpConnPtr& conn) {
        msg = conn->get();
        if (msg == "quit\n") {
            cli.shutdown();
            cli.onCloseBySock(conn);
        } else {
            std::cout << msg;
        }
    }
    void onCloseByPeer(const pqnet::TcpConnPtr& conn) {
        // None
    }
    void onCloseBySock(const pqnet::TcpConnPtr& conn) {
        // None
    }
private:
    std::string msg;
    pqnet::TcpClient cli;
};

int main()
{
    TcpEchoClient echocli("127.0.0.1", 12488);
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
