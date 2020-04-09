# Records for pqnet

记录一些开发 pqnet 的过程中所遇到的错误或异常。

## 2020-04-09

```log
[INFO ] 2020-04-09 14:47:17:119547 httpserver.cpp:42: ConnFd: 13, Func: HttpServer::onConnect
[DEBUG] 2020-04-09 14:47:17:128416 ../../pqnet/core/connection.cpp:76: ConnFd: 13, Func: TcpConnection::handleRead
[INFO ] 2020-04-09 14:47:17:128491 httpserver.cpp:48: ConnFd: 13, Func: HttpServer::onRequest
RequestCnt: 8
[DEBUG] 2020-04-09 14:47:17:129294 ../../pqnet/core/connection.cpp:76: ConnFd: 13, Func: TcpConnection::handleRead
[DEBUG] 2020-04-09 14:47:17:129348 ../../pqnet/core/connection.cpp:32: ConnFd: 13, Func: TcpConnection::connectDestroyed
[INFO ] 2020-04-09 14:47:17:129506 httpserver.cpp:45: ConnFd: 13, Func: HttpServer::onClose
terminate called after throwing an instance of 'std::bad_weak_ptr'
  what():  bad_weak_ptr
Aborted
```

利用浏览器作为HTTP客户端测试httpserver时出现异常，似乎是 shared_from_this() 引起的问题，复现概率极小，先在这里记录一下。
