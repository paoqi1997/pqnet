#include <cstdio>
#include <iostream>

#include <pqnet/core/client.h>
#include <pqnet/core/trigger.h>
#include <pqnet/util/logger.h>
#include <pqnet/util/signal.h>

using std::cin;
using std::cout;
using std::endl;
using namespace std::placeholders;

class TcpEchoClient
{
public:
    TcpEchoClient(const char *servname, std::uint16_t port)
        : cli(servname, port), inTrigger(cli.getLooperFd(), fileno(stdin))
    {
        inTrigger.addToLoop();
        inTrigger.likeReading();
        inTrigger.setReadHandler(std::bind(&TcpEchoClient::handleStdIn, this));
    }
    void start() {
        cli.setConnectCallBack(
            std::bind(&TcpEchoClient::onConnect, this, _1)
        );
        cli.setCloseCallBack(
            std::bind(&TcpEchoClient::onClose, this, _1)
        );
        cli.setMessageArrivedCallBack(
            std::bind(&TcpEchoClient::onMsgArrived, this, _1)
        );
        cli.start();
    }
    void shutdown() {
        cli.shutdown();
    }
    void handleStdIn() {
        INFO("FileFd: %d, Func: TcpEchoClient::%s", fileno(stdin), __func__);
        cin >> msg;
        cli.getConn()->send(msg.c_str(), msg.length());
    }
    void onConnect(const pqnet::TcpConnPtr& conn) {
        INFO("ConnFd: %d, Func: TcpEchoClient::%s", conn->getFd(), __func__);
    }
    void onClose(const pqnet::TcpConnPtr& conn) {
        INFO("ConnFd: %d, Func: TcpEchoClient::%s", conn->getFd(), __func__);
        cli.shutdown();
    }
    void onMsgArrived(const pqnet::TcpConnPtr& conn) {
        INFO("ConnFd: %d, Func: TcpEchoClient::%s", conn->getFd(), __func__);
        msg = conn->getInputBuffer()->get(BUFFERSIZE);
        if (msg == "quit") {
            cli.shutdown();
        } else {
            cout << msg << endl;
        }
    }
private:
    static const std::size_t BUFFERSIZE = 128;
private:
    std::string msg;
    pqnet::TcpClient cli;
    pqnet::Trigger inTrigger;
};

int main()
{
    pqnet::Logger::getLogger()->setLogLevel(pqnet::Logger::DEBUG);
    TcpEchoClient echocli("127.0.0.1", 12488);
    auto SIGINT_HANDLER = [&]{
        echocli.shutdown();
        cout << endl;
        cout << "Captures the signal: SIGINT(2)." << endl;
    };
    auto SIGTERM_HANDLER = [&]{
        echocli.shutdown();
        cout << "Captures the signal: SIGTERM(15)." << endl;
    };
    pqnet::addSignal(SIGINT, SIGINT_HANDLER);
    pqnet::addSignal(SIGTERM, SIGTERM_HANDLER);
    pqnet::waitSig();
    echocli.start();
    return 0;
}
