#pragma once

#include <arpa/inet.h>
#include <string>
#include <cstdint>


class SocketAddress {
private:
    sockaddr_in address_;
    std::string ip_address_;

public:
    SocketAddress();
    SocketAddress(uint16_t port, const std::string& ip_addr);
    static SocketAddress any(uint16_t port);
    static SocketAddress localhost(uint16_t port);
    socklen_t size() const;
    void update_ip();
    const sockaddr* data() const;
    sockaddr* data();
    const std::string& get_ip() const;
    const sockaddr_in& get_sockaddr() const;
};