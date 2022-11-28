#ifndef ASIO_ACCEPTOR_H
#define ASIO_ACCEPTOR_H

#include <netinet/in.h>
#include <channel.h>

namespace asio
{
class InetAddress;

class Acceptor
{
public:
    Acceptor(const InetAddress& addr, EventLoop* loop);
    ~Acceptor();

    void bind(const sockaddr* addr);
    void listen();
    void handle_read();
    void set_new_connection_cb(const std::function<void(int sock_fd, const sockaddr_in& addr)>& cb);
private:
    Acceptor(const Acceptor&) = delete;
    Acceptor operator=(const Acceptor&) = delete;

private:
    int fd_;
    Channel accept_channel_;
    std::function<void(int sock_fd, const sockaddr_in& addr)> new_connection_cb_;
};    
}
#endif