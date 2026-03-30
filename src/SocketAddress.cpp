#include "SocketAddress.hpp"
#include <string.h>
#include <errno.h>
#include <stdexcept>

SocketAddress::SocketAddress(uint16_t port, const std::string& ip_addr) 
    : address_{}
{
    address_.sin_family = AF_INET;
    address_.sin_port = htons(port);
    int res = inet_pton(AF_INET, ip_addr.c_str(), &address_.sin_addr);
    if (res < 0) {
        throw std::runtime_error(std::string("SocketAddress creation failed: ") + strerror(errno));
    }
    else if (!res) {
        throw std::runtime_error("Invalid IP address: " + ip_addr);
    }
}



SocketAddress SocketAddress::any(uint16_t port) {

}