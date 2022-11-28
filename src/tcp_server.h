#ifndef ASIO_TCP_SERVER_H
#define ASIO_TCP_SERVER_H

#include <string>
#include <memory>

namespace asio
{
class EventLoop;
class Acceptor;
class InetAddress;

class TcpServer
{
public:
    TcpServer(const InetAddress& address, EventLoop* loop);
    ~TcpServer();

    void start();
private:
    TcpServer(const TcpServer&) = delete;
    TcpServer operator=(const TcpServer&) = delete;

private:
    EventLoop* loop_ = nullptr;
    std::unique_ptr<Acceptor> accept_;
}; 
}

#endif