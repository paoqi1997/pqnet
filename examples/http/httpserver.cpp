#include <cstdio>
#include <iostream>
#include <vector>

#include <sys/socket.h>

#include <pqnet/core/server.h>
#include <pqnet/util/logger.h>
#include <pqnet/util/signal.h>

using std::cout;
using std::endl;
using namespace std::placeholders;

void shutdownWrite(int sockfd)
{
    if (shutdown(sockfd, SHUT_WR) == -1) {
        ERROR(std::strerror(errno));
    }
}

class HttpServer
{
public:
    HttpServer(std::uint16_t port) : reqcnt(0), serv(port) {}
    void start() {
        serv.setConnectCallBack(
            std::bind(&HttpServer::onConnect, this, _1)
        );
        serv.setCloseCallBack(
            std::bind(&HttpServer::onClose, this, _1)
        );
        serv.setMessageArrivedCallBack(
            std::bind(&HttpServer::onRequest, this, _1)
        );
        serv.start();
    }
    void shutdown() {
        serv.shutdown();
    }
    void onConnect(const pqnet::TcpConnPtr& conn) {
        INFO("ConnFd: %d, Func: HttpServer::%s", conn->getFd(), __func__);
    }
    void onClose(const pqnet::TcpConnPtr& conn) {
        INFO("ConnFd: %d, Func: HttpServer::%s", conn->getFd(), __func__);
    }
    void onRequest(const pqnet::TcpConnPtr& conn) {
        INFO("ConnFd: %d, Func: HttpServer::%s", conn->getFd(), __func__);
        std::string req = conn->getInputBuffer()->get(BUFFERSIZE);
        /*std::vector<std::string> lines;
        for (std::size_t i = 0, j = 0; i < req.length(); ) {
            if (req[j] == '\r' && req[j + 1] == '\n') {
                lines.push_back(req.substr(i, j - i));
                // ...[\r]\n
                // \r\n
                if (req[j + 2] == '\r') {
                    i = j = j + 4;
                }
                // ...[\r]\n
                // ...\r\n
                else {
                    i = j = j + 2;
                }
            } else {
                ++j;
            }
        }
        for (auto& line : lines) {
            cout << line << endl;
        }*/
        std::printf("RequestCnt: %lu\n", ++reqcnt);
        std::string rep = "HTTP/1.1 200 OK\r\n\r\n";
        conn->send(rep.c_str(), rep.length());
        shutdownWrite(conn->getFd());
    }
private:
    static const std::size_t BUFFERSIZE = 1024;
private:
    std::size_t reqcnt;
    pqnet::TcpServer serv;
};

int main()
{
    pqnet::Logger::getLogger()->setLogLevel(pqnet::Logger::DEBUG);
    HttpServer httpserv(8080);
    auto SIGINT_HANDLER = [&]{
        httpserv.shutdown();
        cout << endl;
        cout << "Captures the signal: SIGINT(2)." << endl;
    };
    auto SIGTERM_HANDLER = [&]{
        httpserv.shutdown();
        cout << "Captures the signal: SIGTERM(15)." << endl;
    };
    pqnet::addSignal(SIGINT, SIGINT_HANDLER);
    pqnet::addSignal(SIGTERM, SIGTERM_HANDLER);
    pqnet::waitSig();
    httpserv.start();
    return 0;
}
