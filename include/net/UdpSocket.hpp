#pragma once

#include <span>
#include <sys/types.h>
#include <cstddef>

#include "net/SocketAddress.hpp"

class UdpSocket {
private:
    int socket_fd_;

public:
    UdpSocket();
    void bind_socket(const SocketAddress& addr);
    ssize_t send_to(std::span<const std::byte> buffer, const SocketAddress& addr);
    ssize_t receive_from(std::span<std::byte> buffer, SocketAddress& addr);
};