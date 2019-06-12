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
            std::bind(&TcpEchoClient::onMsgArrived, this, _1)
        );
        cli.run();
    }
    void shutdown() {
        cli.shutdown();
    }
    void handleStdIn() {
        TRACE("%d:%s", cli.getConn()->getFd(), __func__);
        std::cin >> msg;
        cli.getConn()->send(msg.c_str(), msg.length());
    }
    void onConnect(const pqnet::TcpConnPtr& conn) {
        TRACE("%d:%s", conn->getFd(), __func__);
    }
    void onClose(const pqnet::TcpConnPtr& conn) {
        TRACE("%d:%s", conn->getFd(), __func__);
    }
    void onMsgArrived(const pqnet::TcpConnPtr& conn) {
        TRACE("%d:%s", conn->getFd(), __func__);
        msg = conn->getInputBuffer()->get(128);
        if (msg == "quit") {
            cli.shutdown();
        } else {
            std::cout << msg << std::endl;
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
    TcpEchoClient echocli("127.0.0.1", 12488);
    auto SIGINT_HANDLER = [&](){
        echocli.shutdown();
        std::cout << std::endl;
        std::cout << "Captures the signal: SIGINT(2)." << std::endl;
    };
    auto SIGTERM_HANDLER = [&](){
        echocli.shutdown();
        std::cout << "Captures the signal: SIGTERM(15)." << std::endl;
    };
    pqnet::addSignal(SIGINT, SIGINT_HANDLER);
    pqnet::addSignal(SIGTERM, SIGTERM_HANDLER);
    pqnet::waitSig();
    echocli.run();
    return 0;
}
