#include <cstdio>
#include <iostream>

#include <pqnet/core/client.h>
#include <pqnet/core/trigger.h>
#include <pqnet/util/logger.h>
#include <pqnet/util/signal.h>

using namespace std::placeholders;

class TcpEchoClient
{
public:
    TcpEchoClient(const char *servname, std::uint16_t port)
        : cli(servname, port), inTrigger(cli.getEpfd(), fileno(stdin))
    {
        inTrigger.addToLoop();
        inTrigger.likeReading();
        inTrigger.setReadHandler(std::bind(&TcpEchoClient::handleStdIn, this));
    }
    void run() {
        cli.setConnectCallBack(
            std::bind(&TcpEchoClient::onConnect, this, _1)
        );
        cli.setCloseCallBack(
            std::bind(&TcpEchoClient::onClose, this, _1)
        );
        cli.setMessageArrivedCallBack(
            std::bind(&TcpEchoClient::onMessage, this, _1)
        );
        cli.run();
    }
    void shutdown() {
        cli.shutdown();
    }
    void handleStdIn() {
        std::cin >> msg; msg += '\n';
        cli.getConn()->send(msg.c_str(), msg.length());
        TRACE("%d stdin.", cli.getConn()->getFd());
    }
    void onConnect(const pqnet::TcpConnPtr& conn) {
        TRACE("%d connect.", conn->getFd());
    }
    void onClose(const pqnet::TcpConnPtr& conn) {
        TRACE("%d close.", conn->getFd());
    }
    void onMessage(const pqnet::TcpConnPtr& conn) {
        msg = conn->getInputBuffer()->get(128);
        if (msg == "quit\n") {
            cli.shutdown();
        } else {
            std::cout << msg;
        }
    }
private:
    std::string msg;
    pqnet::TcpClient cli;
    pqnet::Trigger inTrigger;
};

int main()
{
    pqnet::Logger::getLogger()->setLogLevel(pqnet::Logger::TRACE);
    pqnet::Logger::getLogger()->setOutput(pqnet::Logger::CONSOLE);
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
