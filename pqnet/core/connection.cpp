#include <cerrno>
#include <cstring>
#include <functional>

#include <unistd.h>

#include "../util/logger.h"
#include "connection.h"

using namespace pqnet;

TcpConnection::TcpConnection(int epfd, int fd) : tg(new Trigger(epfd, fd))
{
    tg->setReadHandler(std::bind(&TcpConnection::handleRead, this));
    tg->setWriteHandler(std::bind(&TcpConnection::handleWrite, this));
}

void TcpConnection::connectEstablished()
{
    TRACE("%d:%s", tg->getFd(), __func__);
    tg->addToLoop();
    tg->likeReading();
    if (conncb) {
        conncb(shared_from_this());
    }
    connected = true;
}

void TcpConnection::connectDestroyed()
{
    TRACE("%d:%s", tg->getFd(), __func__);
    tg->removeFromLoop();
    if (close(tg->getFd()) != 0) {
        ERROR(std::strerror(errno));
    }
    if (closecb) {
        closecb(shared_from_this());
    }
    connected = false;
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
            flag = false;
            ERROR(std::strerror(errno));
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
    TRACE("%d:%s", tg->getFd(), __func__);
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
