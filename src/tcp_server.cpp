
#include <event_loop.h>
#include <logging.h>
#include <acceptor.h>
#include <inet_address.h>
#include <tcp_server.h>

namespace asio
{
TcpServer::TcpServer(const InetAddress& address, EventLoop* loop)
  : accept_(new Acceptor(address, loop)), 
    loop_(loop)
{
}

TcpServer::~TcpServer()
{

}

void TcpServer::start()
{
    loop_->run_in_loop(std::bind(&Acceptor::listen, accept_.get()));
}

}