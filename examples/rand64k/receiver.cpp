#include <cstdio>
#include <iostream>
#include <string>

#include <pqnet/core/server.h>
#include <pqnet/util/logger.h>
#include <pqnet/util/signal.h>

#include "package.h"

using std::cout;
using std::endl;
using namespace std::placeholders;

class Receiver
{
public:
    Receiver(std::uint16_t port) : serv(port) {}
    void start() {
        serv.setConnectCallBack(
            std::bind(&Receiver::onConnect, this, _1)
        );
        serv.setCloseCallBack(
            std::bind(&Receiver::onClose, this, _1)
        );
        serv.setMessageArrivedCallBack(
            std::bind(&Receiver::onMsgArrived, this, _1)
        );
        serv.start();
    }
    void shutdown() {
        serv.shutdown();
    }
    void onConnect(const pqnet::TcpConnPtr& conn) {
        INFO("ConnFd: %d, Func: Receiver::%s", conn->getFd(), __func__);
    }
    void onClose(const pqnet::TcpConnPtr& conn) {
        INFO("ConnFd: %d, Func: Receiver::%s", conn->getFd(), __func__);
    }
    void onMsgArrived(const pqnet::TcpConnPtr& conn) {
        INFO("ConnFd: %d, Func: Receiver::%s", conn->getFd(), __func__);

        std::int32_t protocolNo = conn->getInputBuffer()->getInt32();
        if (protocolNo == PROTOCOL_NO) {
            std::int32_t datalen = conn->getInputBuffer()->getInt32();

            std::string data = conn->getInputBuffer()->get(datalen);
            std::string begin = data.substr(0, 3);
            std::string end = data.substr(data.length() - 3, 3);
            std::printf("RecvDataLen: %d, Data: {%s...%s}\n", datalen, begin.c_str(), end.c_str());

            conn->send(reinterpret_cast<char*>(&datalen), sizeof(datalen));
        }
    }
private:
    pqnet::TcpServer serv;
};

int main()
{
    pqnet::Logger::getInstance()->setLogLevel(pqnet::Logger::DEBUG);
    Receiver receiver(12358);
    auto SIGINT_HANDLER = [&]{
        receiver.shutdown();
        cout << endl;
        cout << "Captures the signal: SIGINT(2)." << endl;
    };
    auto SIGTERM_HANDLER = [&]{
        receiver.shutdown();
        cout << "Captures the signal: SIGTERM(15)." << endl;
    };
    pqnet::addSignal(SIGINT, SIGINT_HANDLER);
    pqnet::addSignal(SIGTERM, SIGTERM_HANDLER);
    pqnet::waitSig();
    receiver.start();
    return 0;
}
