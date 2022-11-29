#ifndef ASIO_TCP_SERVER_H
#define ASIO_TCP_SERVER_H

#include <string>
#include <memory>
#include <map>

namespace asio
{
class EventLoop;
class Acceptor;
class InetAddress;
class TcpConnection;

class TcpServer
{
public:
    TcpServer(const InetAddress& address, EventLoop* loop);
    ~TcpServer();

    void start();
private:
    TcpServer(const TcpServer&) = delete;
    TcpServer operator=(const TcpServer&) = delete;

    void new_connection(int sock_fd, const sockaddr_in& addr);
private:
    EventLoop* loop_ = nullptr;
    EventLoop connect_loop_;
    std::unique_ptr<Acceptor> accept_;
    std::map<int, std::shared_ptr<TcpConnection>> connect_map_;
}; 
}

#endif