#include <iostream>

#include <pqnet/core/server.h>
#include <pqnet/util/logger.h>
#include <pqnet/util/signal.h>

using namespace std::placeholders;

class TcpEchoServer
{
public:
    TcpEchoServer(std::uint16_t port) : serv(port) {}
    void start() {
        serv.setConnectCallBack(
            std::bind(&TcpEchoServer::onConnect, this, _1)
        );
        serv.setCloseCallBack(
            std::bind(&TcpEchoServer::onClose, this, _1)
        );
        serv.setMessageArrivedCallBack(
            std::bind(&TcpEchoServer::onMsgArrived, this, _1)
        );
        serv.start();
    }
    void shutdown() {
        serv.shutdown();
    }
    void onConnect(const pqnet::TcpConnPtr& conn) {
        INFO("Fd: %d, Func: TcpEchoServer::%s", conn->getFd(), __func__);
    }
    void onClose(const pqnet::TcpConnPtr& conn) {
        INFO("Fd: %d, Func: TcpEchoServer::%s", conn->getFd(), __func__);
    }
    void onMsgArrived(const pqnet::TcpConnPtr& conn) {
        INFO("Fd: %d, Func: TcpEchoServer::%s", conn->getFd(), __func__);
        std::string msg = conn->getInputBuffer()->get(128);
        conn->send(msg.c_str(), msg.length());
    }
private:
    pqnet::TcpServer serv;
};

int main()
{
    pqnet::Logger::getLogger()->setLogLevel(pqnet::Logger::DEBUG);
    TcpEchoServer echoserv(12488);
    auto SIGINT_HANDLER = [&]{
        echoserv.shutdown();
        std::cout << std::endl;
        std::cout << "Captures the signal: SIGINT(2)." << std::endl;
    };
    auto SIGTERM_HANDLER = [&]{
        echoserv.shutdown();
        std::cout << "Captures the signal: SIGTERM(15)." << std::endl;
    };
    pqnet::addSignal(SIGINT, SIGINT_HANDLER);
    pqnet::addSignal(SIGTERM, SIGTERM_HANDLER);
    pqnet::waitSig();
    echoserv.start();
    return 0;
}
