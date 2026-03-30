#include "net/SocketAddress.hpp"
#include <string.h>
#include <errno.h>
#include <stdexcept>


SocketAddress::SocketAddress() : address_{} {

}

SocketAddress::SocketAddress(uint16_t port, const std::string& ip_addr) : address_{} {
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
    SocketAddress addr{};
    addr.address_.sin_family = AF_INET;
    addr.address_.sin_port = htons(port);
    addr.address_.sin_addr.s_addr = INADDR_ANY;
    return addr;
}

SocketAddress SocketAddress::localhost(uint16_t port) {
    return SocketAddress(port, "127.0.0.1");
}

socklen_t SocketAddress::size() const {
    return sizeof(address_);
}

const sockaddr* SocketAddress::data() const {
    return reinterpret_cast<const sockaddr*>(&address_);
}

sockaddr* SocketAddress::data() {
    return reinterpret_cast<sockaddr*>(&address_);
}