#include <cerrno>
#include <cstring>
#include <functional>

#include <unistd.h>

#include "../util/logger.h"
#include "connection.h"

using namespace pqnet;

TcpConnection::TcpConnection(int epfd, int _connfd)
    : connfd(_connfd), ch(new Channel(epfd, _connfd))
{
    ch->setReadCallBack(std::bind(&TcpConnection::handleRead, this));
    ch->setWriteCallBack(std::bind(&TcpConnection::handleWrite, this));
    ch->addToLoop();
    ch->likeReading();
}

void TcpConnection::send(const char *data, std::size_t len)
{
    bool flag = true;
    ssize_t n = 0;
    ssize_t remain = len;
    if (!ch->isLikeWriting() && outputBuffer.readableBytes() == 0) {
        n = write(ch->getFd(), data, len);
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
        if (!ch->isLikeWriting()) {
            // For TcpConnection::handleWrite
            ch->likeWriting();
        }
    }
}

void TcpConnection::handleRead()
{
    ssize_t n = inputBuffer.readFrom(ch->getFd(), inputBuffer.writableBytes());
    if (n > 0) {
        if (msgcb) {
            msgcb(shared_from_this());
        }
    } else if (n == 0) {
        handleClose();
    } else {
        ERROR(std::strerror(errno));
    }
}

void TcpConnection::handleWrite()
{
    ssize_t n = outputBuffer.writeTo(ch->getFd(), outputBuffer.readableBytes());
    if (n > 0) {
        if (outputBuffer.readableBytes() == 0) {
            ch->omitWriting();
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
    ch->removeFromLoop();
    if (closecb) {
        closecb(shared_from_this());
    }
}
