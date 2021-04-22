#include <cstdio>
#include <iostream>
#include <string>

#include <pqnet/core/client.h>
#include <pqnet/util/logger.h>
#include <pqnet/util/signal.h>

#include "user.pb.h"

using std::cout;
using std::endl;
using namespace std::placeholders;

class Sender
{
public:
    Sender(const char *servername, std::uint16_t port)
        : cli(servername, port) {}
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

        UserInfo userinfo;

        userinfo.set_uid(185763);
        userinfo.set_name("paoqi");
        userinfo.set_level(23);

        std::string s;
        userinfo.SerializeToString(&s);

        std::int32_t datalen = s.length();

        char sendBuf[32];

        std::memcpy(sendBuf, &datalen, sizeof(datalen));
        std::strcpy(sendBuf + sizeof(std::int32_t), s.c_str());

        conn->send(sendBuf, sizeof(datalen) + s.length());

        std::printf("Send the userinfo to the receiver.\n");
    }
    void onClose(const pqnet::TcpConnPtr& conn) {
        INFO("ConnFd: %d, Func: Sender::%s", conn->getFd(), __func__);
        cli.shutdown();
    }
    void onMsgArrived(const pqnet::TcpConnPtr& conn) {
        INFO("ConnFd: %d, Func: Sender::%s", conn->getFd(), __func__);
    }
private:
    pqnet::TcpClient cli;
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
