#include <arpa/inet.h>
#include <event_loop.h>
#include <logging.h>
#include <acceptor.h>
#include <inet_address.h>
#include <tcp_connection.h>
#include <tcp_server.h>

namespace asio
{
TcpServer::TcpServer(const InetAddress& address, EventLoop* loop)
  : accept_(new Acceptor(address, loop)), 
    connect_loop_(),
    loop_(loop)
{
    accept_->set_new_connection_cb(std::bind(&TcpServer::new_connection, this, std::placeholders::_1, std::placeholders::_2));
}

TcpServer::~TcpServer()
{

}

void TcpServer::start()
{
    loop_->run_in_loop(std::bind(&Acceptor::listen, accept_.get()));
}

void TcpServer::new_connection(int sock_fd, const sockaddr_in& addr)
{
    LOG_INFO("new connect ip=%s, port=%d", inet_ntoa(addr.sin_addr), addr.sin_port);

    std::shared_ptr<TcpConnection> conn(new TcpConnection(&connect_loop_, sock_fd));
    
    connect_map_[sock_fd] = conn;
    connect_loop_.run_in_loop(std::bind(&TcpConnection::connect_established, conn));
}

}