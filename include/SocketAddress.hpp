#pragma once

#include <arpa/inet.h>
#include <string>

struct SocketAddress {
private:
    sockaddr_in address_;
public:
    SocketAddress(uint16_t port, const std::string& ip_addr);
    static SocketAddress any(uint16_t port);
    static SocketAddress localhost(uint16_t port);
    socklen_t size() const;
    socklen_t* size();
    const sockaddr* data() const;
    sockaddr* data();
};