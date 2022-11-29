#ifndef ASIO_TCP_CONNECTION_H
#define ASIO_TCP_CONNECTION_H

#include <memory>

namespace asio
{

class Channel;
class EventLoop;

class TcpConnection
{
public:
    TcpConnection(EventLoop* loop, int sockfd);
    ~TcpConnection();

    void connect_established();
private:
    TcpConnection(const TcpConnection&) = delete;
    TcpConnection operator=(const TcpConnection&) = delete;

    void handle_read();
private:
    EventLoop* loop_;
    std::unique_ptr<Channel> channel_;

};    

}

#endif