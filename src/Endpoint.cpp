#include "Endpoint.hpp"
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <cerrno>


Endpoint::Endpoint() {
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        handle_error();
    }
}

void Endpoint::handle_error() {
    throw std::runtime_error(strerror(errno));
}

void Endpoint::initialize_sock_addr(int _port, sockaddr_in& _sockaddr, in_addr_t _ipaddr) {
    _sockaddr.sin_port = htons(_port);
    _sockaddr.sin_family = AF_INET;
    _sockaddr.sin_addr.s_addr = _ipaddr;
}

void Endpoint::initialize_sock_addr(int _port, sockaddr_in& _sockaddr, const std::string& _ipaddr) {
    _sockaddr.sin_port = htons(_port);
    _sockaddr.sin_family = AF_INET;
    int p = inet_pton(AF_INET, _ipaddr.c_str(), &_sockaddr.sin_addr);
    if (p < 0) {
        handle_error();
    }
    else if (!p) {
        throw std::runtime_error("Invalid IP address");
    }
}

void Endpoint::bind_socket(int _fd, const sockaddr_in& _sockaddr) {
    if (bind(_fd, reinterpret_cast<const sockaddr*>(&_sockaddr), sizeof(_sockaddr)) < 0) {
        handle_error();
    }
}

std::string Endpoint::get_sockaddr_ip(const sockaddr_in& _sockaddr) {
    char addr_ip[INET_ADDRSTRLEN];
    if(inet_ntop(AF_INET, &_sockaddr.sin_addr, addr_ip, sizeof(addr_ip)) == nullptr) {
        handle_error();
    }
    return addr_ip;
}

ssize_t Endpoint::recv_bytes(int _fd, std::span<std::byte>& _buffer, sockaddr_in& _sockaddr) {
    socklen_t len = sizeof(_sockaddr);
    ssize_t n = recvfrom(_fd, _buffer.data(), _buffer.size() - 1, 0, reinterpret_cast<sockaddr*>(&_sockaddr), &len);
    if (n < 0) {
        handle_error();
    }
    return n;
}

ssize_t Endpoint::send_bytes(int _fd, std::span<std::byte>& _buffer, size_t n, const sockaddr_in& _sockaddr) {
    ssize_t sent = sendto(_fd, _buffer.data(), n, 0, reinterpret_cast<const sockaddr*>(&_sockaddr), sizeof(_sockaddr));
    return sent;
}