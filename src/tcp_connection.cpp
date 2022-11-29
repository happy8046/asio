#include <sys/uio.h>
#include <channel.h>
#include <logging.h>
#include <tcp_connection.h>

namespace asio
{

TcpConnection::TcpConnection(EventLoop* loop, int sockfd)
  : loop_(loop), channel_(new Channel(loop))
{
    channel_->set_fd(sockfd);
    channel_->set_read_callback(std::bind(&TcpConnection::handle_read, this));
}

TcpConnection::~TcpConnection()
{
    channel_->disable_all();
    channel_->remove();
}

void TcpConnection::connect_established()
{
    channel_->enable_reading();
}

void TcpConnection::handle_read()
{
    char buff[64] = {0};
    struct iovec vec[1];
    
    vec->iov_base = buff;
    vec->iov_len = 64;
    ssize_t len = ::readv(channel_->get_fd(), vec, 1);
    LOG_INFO("read data len:%ld data:%s", len, buff);
}

}