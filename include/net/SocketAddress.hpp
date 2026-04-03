#pragma once

#include <arpa/inet.h>
#include <string>
#include <cstdint>


class SocketAddress {
private:
    sockaddr_in address_;

public:
    SocketAddress();
    SocketAddress(uint16_t port, const std::string& ip_addr);
    static SocketAddress any(uint16_t port);
    static SocketAddress localhost(uint16_t port);
    socklen_t size() const;
    const sockaddr* data() const;
    sockaddr* data();
    std::string get_ip() const;
};