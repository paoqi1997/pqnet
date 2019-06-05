#include <cerrno>
#include <cstring>
#include <functional>

#include <unistd.h>

#include "../util/logger.h"
#include "connection.h"

using namespace pqnet;

TcpConnection::TcpConnection(int epfd, int _fd)
    : fd(_fd), tg(new Trigger(epfd, _fd))
{
    tg->setReadHandler(std::bind(&TcpConnection::handleRead, this));
    tg->setWriteHandler(std::bind(&TcpConnection::handleWrite, this));
}

void TcpConnection::connectEstablished()
{
    tg->addToLoop();
    tg->likeReading();
    if (conncb) {
        conncb(shared_from_this());
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
            flag = false;
            ERROR(std::strerror(errno));
        }
    }
    if (flag && remain > 0) {
        outputBuffer.append(data + n, remain);
        if (!tg->isLikeWriting()) {
            // For TcpConnection::handleWrite
            tg->likeWriting();
        }
    }
}

void TcpConnection::handleRead()
{
    ssize_t n = inputBuffer.readFrom(tg->getFd(), inputBuffer.writableBytes());
    TRACE("%d:%d handleRead.", getFd(), n);
    if (n > 0) {
        if (macb) {
            TRACE("%d macb.", getFd());
            macb(shared_from_this());
        }
    } else if (n == 0) {
        handleClose();
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

void TcpConnection::handleClose()
{
    tg->removeFromLoop();
    if (closecb) {
        closecb(shared_from_this());
    }
}
