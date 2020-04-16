#include <cerrno>
#include <cstdio>
#include <cstring>
#include <iostream>

#include <pqnet/core/server.h>
#include <pqnet/core/socket.h>
#include <pqnet/core/http/request.h>
#include <pqnet/core/http/response.h>
#include <pqnet/core/http/router.h>
#include <pqnet/util/logger.h>
#include <pqnet/util/signal.h>

using std::cout;
using std::endl;
using namespace std::placeholders;

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
        // Request
        pqnet::http::HttpRequest oHttpReq(req);
        std::printf("RequestCnt: %lu\n", ++reqcnt);
        // Response
        pqnet::http::HttpResponse oHttpRep;
        // Router
        auto hd = router.getHandler("/");
        hd(oHttpReq, oHttpRep);
        std::string rep = oHttpRep.getResponse();
        conn->send(rep.c_str(), rep.length());
        if (pqnet::shutdownWrite(conn->getFd()) == -1) {
            ERROR(std::strerror(errno));
        }
    }
private:
    static const std::size_t BUFFERSIZE = 1024;
private:
    std::size_t reqcnt;
    pqnet::TcpServer serv;
    pqnet::http::HttpRouter router;
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
