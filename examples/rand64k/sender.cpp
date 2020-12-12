#include <cstdio>
#include <iostream>
#include <string>

#include <pqnet/core/client.h>
#include <pqnet/util/logger.h>
#include <pqnet/util/signal.h>

#include "package.h"

using std::cout;
using std::endl;
using namespace std::placeholders;

class Sender
{
public:
    Sender(const char *servername, std::uint16_t port)
        : cli(servername, port), package(PROTOCOL_NO), remainSize(64 * _1_0_KB) {}
    void start() {
        cli.setConnectCallBack(
            std::bind(&Sender::onConnect, this, _1)
        );
        cli.setCloseCallBack(
            std::bind(&Sender::onClose, this, _1)
        );
        cli.setMessageArrivedCallBack(
            std::bind(&Sender::onMsgArrived, this, _1)
        );
        cli.start();
    }
    void shutdown() {
        cli.shutdown();
    }
    void onConnect(const pqnet::TcpConnPtr& conn) {
        INFO("ConnFd: %d, Func: Sender::%s", conn->getFd(), __func__);

        package.refreshData();
        conn->send(package.p(), package.length());

        std::string begin = package.begin3();
        std::string end = package.end3();
        std::printf("SendDataLen: %zu, Data: {%s...%s}\n", package.dataLength(), begin.c_str(), end.c_str());
    }
    void onClose(const pqnet::TcpConnPtr& conn) {
        INFO("ConnFd: %d, Func: Sender::%s", conn->getFd(), __func__);

        if (remainSize > 0) {
            cout << "oops!" << endl;
            cli.shutdown();
        }
    }
    void onMsgArrived(const pqnet::TcpConnPtr& conn) {
        INFO("ConnFd: %d, Func: Sender::%s", conn->getFd(), __func__);

        std::int32_t datalen = conn->getInputBuffer()->getInt32();
        std::printf("RecvDataLen: %d\n", datalen);

        if (static_cast<int>(remainSize - datalen) <= 0) {
            remainSize = 0;
            cout << "finished." << endl;
            cli.shutdown();
        } else {
            remainSize -= datalen;
            std::printf("RemainSize: %zu\n", remainSize);

            package.refreshData();
            conn->getEventLoop()->runAfter([conn, this](void*){
                conn->send(package.p(), package.length());
            }, nullptr, 1000);

            std::string begin = package.begin3();
            std::string end = package.end3();
            std::printf("SendDataLen: %zu, Data: {%s...%s}\n", package.dataLength(), begin.c_str(), end.c_str());
        }
    }
private:
    pqnet::TcpClient cli;
    Package package;
    std::size_t remainSize;
};

int main()
{
    pqnet::Logger::getInstance()->setLogLevel(pqnet::Logger::DEBUG);
    Sender sender("127.0.0.1", 12358);
    auto SIGINT_HANDLER = [&]{
        sender.shutdown();
        cout << endl;
        cout << "Captures the signal: SIGINT(2)." << endl;
    };
    auto SIGTERM_HANDLER = [&]{
        sender.shutdown();
        cout << "Captures the signal: SIGTERM(15)." << endl;
    };
    pqnet::addSignal(SIGINT, SIGINT_HANDLER);
    pqnet::addSignal(SIGTERM, SIGTERM_HANDLER);
    pqnet::waitSig();
    sender.start();
    return 0;
}
