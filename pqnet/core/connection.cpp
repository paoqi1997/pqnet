#include <cerrno>
#include <cstring>
#include <functional>

#include <unistd.h>

#include "../util/logger.h"
#include "connection.h"

using namespace pqnet;

TcpConnection::TcpConnection(EventLoop *_looper, int fd)
    : looper(_looper), tg(new Trigger(_looper->getFd(), fd))
{
    tg->setReadHandler(std::bind(&TcpConnection::handleRead, this));
    tg->setWriteHandler(std::bind(&TcpConnection::handleWrite, this));
}

void TcpConnection::connectEstablished()
{
    DEBUG("ConnFd: %d, Func: TcpConnection::%s", tg->getFd(), __func__);
    connected = true;
    tg->addToLoop();
    tg->likeReading();
    if (conncb) {
        conncb(shared_from_this());
    }
}

void TcpConnection::connectDestroyed()
{
    DEBUG("ConnFd: %d, Func: TcpConnection::%s", tg->getFd(), __func__);
    tg->removeFromLoop();
    if (close(tg->getFd()) != 0) {
        ERROR(std::strerror(errno));
    }
    connected = false;
    if (closecb) {
        closecb(shared_from_this());
    }
    if (iccb) {
        iccb(shared_from_this());
    }
}

void TcpConnection::send(const char *data, std::size_t len)
{
    bool flag = true;
    ssize_t n = 0;
    ssize_t remain = len;
    if (!tg->isLikeWriting() && outputBuffer.readableBytes() == 0) {
        n = write(tg->getFd(), data, len);
        if (n > 0) {
            remain = len - n;
            if (remain == 0 && wccb) {
                if (wccb) {
                    wccb(shared_from_this());
                }
            }
        } else {
            ERROR(std::strerror(errno));
            flag = false;
        }
    }
    if (flag && remain > 0) {
        outputBuffer.append(data + n, remain);
        if (!tg->isLikeWriting()) {
            // for TcpConnection::handleWrite
            tg->likeWriting();
        }
    }
}

void TcpConnection::handleRead()
{
    DEBUG("ConnFd: %d, Func: TcpConnection::%s", tg->getFd(), __func__);
    ssize_t n = inputBuffer.readFrom(tg->getFd(), inputBuffer.writableBytes());
    if (n > 0) {
        if (macb) {
            macb(shared_from_this());
        }
    } else if (n == 0) {
        this->connectDestroyed();
    } else {
        ERROR(std::strerror(errno));
    }
}

void TcpConnection::handleWrite()
{
    ssize_t n = outputBuffer.writeTo(tg->getFd(), outputBuffer.readableBytes());
    if (n > 0) {
        if (outputBuffer.readableBytes() == 0) {
            tg->omitWriting();
            if (wccb) {
                wccb(shared_from_this());
            }
        }
    } else {
        ERROR(std::strerror(errno));
    }
}
