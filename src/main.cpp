#include <unistd.h>
#include <inet_address.h>
#include <event_loop.h>
#include <tcp_server.h>

int main()
{
    asio::InetAddress address("127.0.0.1", 8989, false);
    asio::EventLoop loop;
    asio::TcpServer server(address, &loop);

    server.start();
    loop.loop();
    
    return 0;
}