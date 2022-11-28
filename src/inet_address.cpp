#include <arpa/inet.h>
#include <inet_address.h>
#include <string.h>
#include <util.h>

namespace asio
{
InetAddress::InetAddress(const std::string& ip, uint16_t port, bool is_ipv6)
{
    if (is_ipv6)
    {
        // FIXME: not support ipv6
        // memset(&addr6_, 0, sizeof(addr6_));
        // addr6_.sin6_family = AF_INET6;
        // addr6_.sin6_port = port;
        // addr6_.sin6_addr = 
    }
    else
    {
        memset(&addr_, 0, sizeof(addr_));
        addr_.sin_family = AF_INET;
        addr_.sin_port = htobe16(port);
        addr_.sin_addr.s_addr = inet_addr(ip.c_str());
    }
}

InetAddress::~InetAddress()
{

}

std::string InetAddress::ip() const
{
    return inet_ntoa(addr_.sin_addr);
}

uint16_t InetAddress::port() const
{
    return be16toh(addr_.sin_port);
}

const sockaddr* InetAddress::get_sockaddr() const
{
    return static_cast<const sockaddr*>(implicit_cast<const void*>(&addr_));
}

}