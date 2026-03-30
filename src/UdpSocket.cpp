#include "UdpSocket.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <stdexcept>

UdpSocket::UdpSocket() {
    socket_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd_ < 0) {
        throw std::runtime_error(std::string("Socket creation failed: ") + strerror(errno));
    }
}

void UdpSocket::bind_socket(const SocketAddress& addr) {
    if (bind(socket_fd_, addr.data(), sizeof(addr)) < 0) {
        throw std::runtime_error(std::string("bind_socket failed: ") + strerror(errno));
    }
}

ssize_t UdpSocket::send_to(std::span<const std::byte> buffer, const SocketAddress& addr) {
    const std::byte* payload = reinterpret_cast<const std::byte*>(buffer.data());
    ssize_t sent = sendto(socket_fd_, payload, buffer.size(), 0, addr.data(), addr.size());
    if (sent < 0) {
        throw std::runtime_error(std::string("send_to failed: ") + strerror(errno));
    }
    return sent;
}

ssize_t UdpSocket::receive_from(std::span<std::byte> buffer, SocketAddress& addr) {
    std::byte* buffer_ptr = reinterpret_cast<std::byte*>(buffer.data());
    socklen_t len = addr.size();
    ssize_t n = recvfrom(socket_fd_, buffer_ptr, sizeof(buffer), 0, addr.data(), &len);
    if (n < 0) {
        throw std::runtime_error(std::string("receive_from failed: ") + strerror(errno));
    }
    return n;
}