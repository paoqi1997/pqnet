#include <iostream>

#include "../../ipaddr.h"

int main()
{
    pqnet::Ip4Addr ipaddr1(8088);
    pqnet::Ip4Addr ipaddr2("www.baidu.com", 443);

    std::cout << ipaddr1.ipInt() << std::endl;
    std::cout << ipaddr1.ip() << ' ' << ipaddr1.port() << ' ' << ipaddr1.toString() << std::endl;
    std::cout << ipaddr2.ipInt() << std::endl;
    std::cout << ipaddr2.ip() << ' ' << ipaddr2.port() << ' ' << ipaddr2.toString() << std::endl;

    return 0;
}
