#include <iostream>

#include <pqnet/core/server.h>
#include <pqnet/util/logger.h>
#include <pqnet/util/signal.h>

using namespace std::placeholders;

class TcpEchoServer
{
public:
    TcpEchoServer(std::uint16_t port) : serv(port) {}
    void run() {
        serv.setConnectCallBack(
            std::bind(&TcpEchoServer::onConnect, this, _1)
        );
        serv.setCloseCallBack(
            std::bind(&TcpEchoServer::onClose, this, _1)
        );
        serv.setMessageArrivedCallBack(
            std::bind(&TcpEchoServer::onMessage, this, _1)
        );
        serv.run();
    }
    void shutdown() {
        serv.shutdown();
    }
    void onConnect(const pqnet::TcpConnPtr& conn) {
        TRACE("%d connect.", conn->getFd());
    }
    void onClose(const pqnet::TcpConnPtr& conn) {
        TRACE("%d close.", conn->getFd());
    }
    void onMessage(const pqnet::TcpConnPtr& conn) {
        std::string msg = conn->getInputBuffer()->get(128);
        conn->send(msg.c_str(), msg.length());
        TRACE("%d onMessage.", conn->getFd());
    }
private:
    pqnet::TcpServer serv;
};

int main()
{
    pqnet::Logger::getLogger()->setLogLevel(pqnet::Logger::TRACE);
    pqnet::Logger::getLogger()->setOutput(pqnet::Logger::CONSOLE);
    TcpEchoServer echoserv(12488);
    auto SIGINT_HANDLER = [&](){
        echoserv.shutdown();
        std::cout << std::endl;
        std::cout << "Exit echo server." << std::endl;
    };
    auto SIGTERM_HANDLER = [&](){
        echoserv.shutdown();
        std::cout << "Exit echo server." << std::endl;
    };
    pqnet::addSignal(SIGINT, SIGINT_HANDLER);
    pqnet::addSignal(SIGTERM, SIGTERM_HANDLER);
    pqnet::waitSig();
    echoserv.run();
    return 0;
}
