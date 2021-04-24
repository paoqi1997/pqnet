#include <cstdio>
#include <iostream>
#include <string>

#include <pqnet/core/client.h>
#include <pqnet/util/logger.h>
#include <pqnet/util/signal.h>

#include "package.h"
#include "player.pb.h"

using std::cout;
using std::endl;
using namespace std::placeholders;

class Sender
{
public:
    Sender(const char *servername, std::uint16_t port)
        : cli(servername, port), package(PROTOCOL_NO) {}
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

        conn->getEventLoop()->runEvery([conn, this](void*){
            std::uint32_t uid = RandomUID();
            std::string name = RandomName();
            std::uint32_t level = RandomLevel();

            playerinfo.set_uid(uid);
            playerinfo.set_name(name);
            playerinfo.set_level(level);

            playerinfo.SerializeToString(&data);

            package.packString(data);

            conn->send(package.p(), package.length());

            package.clearData();

            std::printf("send { uid=%d, name=%s(%zu), level=%d }\n",
                uid, name.c_str(), name.length(), level);
        }, nullptr, 0, 1000);
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
    std::string data;
    Package package;
    PlayerInfo playerinfo;
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
