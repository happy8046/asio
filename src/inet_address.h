#ifndef ASIO_INET_ADDRESS_H
#define ASIO_INET_ADDRESS_H

#include <netinet/in.h>
#include <stdint.h>
#include <string>

namespace asio
{
class InetAddress
{
public:
    InetAddress(const std::string& ip, uint16_t port, bool is_ipv6);
    ~InetAddress();

    std::string ip() const;
    uint16_t port() const;
    const sockaddr* get_sockaddr() const;
private:
    InetAddress(const InetAddress&) = delete;
    InetAddress operator=(const InetAddress&) = delete;

private:

    union 
    {
        sockaddr_in addr_;
        sockaddr_in6 addr6_;
    };
    
};

}
#endif