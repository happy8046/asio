#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <logging.h>
#include <inet_address.h>
#include <acceptor.h>

namespace asio
{
Acceptor::Acceptor(const InetAddress& addr, EventLoop* loop)
    : accept_channel_(loop)
{
    fd_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(fd_ >= 0);
    bind(addr.get_sockaddr());

    accept_channel_.set_fd(fd_);
    accept_channel_.set_read_callback(std::bind(&Acceptor::handle_read, this));
}

Acceptor::~Acceptor()
{ 
    accept_channel_.disable_all();
    accept_channel_.remove();
    ::close(fd_);
}

void Acceptor::bind(const sockaddr* addr)
{
    sockaddr_in* ipv4_addr = (sockaddr_in*)addr;
    LOG_INFO("bind ip:%s, port:%d", inet_ntoa(ipv4_addr->sin_addr), ipv4_addr->sin_port);

    int ret = ::bind(fd_, addr, sizeof(*addr));

    if (ret < 0)
    {
        LOG_INFO("bind error: %d", ret);
    }
}

void Acceptor::listen()
{
    int ret = ::listen(fd_, SOMAXCONN);

    if (ret < 0)
    {
        LOG_INFO("listen error %d", ret);
    }
    accept_channel_.enable_reading();
}

void Acceptor::handle_read()
{
    sockaddr_in addr;
    socklen_t sock_len = static_cast<socklen_t>(sizeof addr);

    memset(&addr, 0, sizeof addr);
    int connfd = ::accept4(fd_, (sockaddr*)&addr, &sock_len, SOCK_NONBLOCK | SOCK_CLOEXEC);

    if (connfd > 0)
    {
        LOG_INFO("new connect ip=%s, port=%d", inet_ntoa(addr.sin_addr), addr.sin_port);
        if (new_connection_cb_)
        {
            new_connection_cb_(connfd, addr);
        }
        else
        {
            if (::close(connfd) < 0)
            {
                LOG_INFO("sockets::close error %d", connfd);
            }
        }
    }
}

void Acceptor::set_new_connection_cb(const std::function<void(int sock_fd, const sockaddr_in& addr)>& cb)
{
    new_connection_cb_ = cb;
}

}