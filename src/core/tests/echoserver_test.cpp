#include <iostream>

#include "../callback.h"
#include "../server.h"

static bool done = false;

void onConnect(const pqnet::TcpConnPtr& conn) { conn->send("Hello!\n"); }
void onClose(const pqnet::TcpConnPtr& conn) { conn->send("Bye!\n"); }
void onRead(const pqnet::TcpConnPtr& conn) { conn->recv(); }
void onMessage(const pqnet::TcpConnPtr& conn) { conn->send(); }

int main()
{
    pqnet::TcpServer serv(12488, done);
    serv.setConnectCallBack(onConnect);
    serv.setCloseCallBack(onClose);
    serv.setReadCallBack(onRead);
    serv.setMessageCallBack(onMessage);
    serv.run();
    return 0;
}
