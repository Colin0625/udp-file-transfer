#include "Endpoint.hpp"
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <vector>
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

void Endpoint::initialize_server_addr(int _port, in_addr_t _ipaddr) {
    server_addr.sin_port = htons(_port);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = _ipaddr;
}

void Endpoint::initialize_server_addr(int _port, const std::string& _ipaddr) {
    server_addr.sin_port = htons(_port);
    server_addr.sin_family = AF_INET;
    int p = inet_pton(AF_INET, _ipaddr.c_str(), &server_addr.sin_addr);
    if (p < 0) {
        handle_error();
    }
    else if (!p) {
        throw std::runtime_error("Invalid IP address");
    }
}

void Endpoint::bind_socket() {
    if (bind(socket_fd, reinterpret_cast<const sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
        handle_error();
    }
}

std::string Endpoint::get_sockaddr_ip(const sockaddr_in& _sockaddr) {
    char addr_ip[INET_ADDRSTRLEN];
    if(inet_ntop(AF_INET, &server_addr.sin_addr, addr_ip, sizeof(addr_ip)) == nullptr) {
        handle_error();
    }
    return addr_ip;
}

ssize_t Endpoint::recv_bytes(std::span<std::byte> _buffer, sockaddr_in& _sockaddr) {
    socklen_t len = sizeof(server_addr);
    ssize_t n = recvfrom(socket_fd, _buffer.data(), _buffer.size() - 1, 0, reinterpret_cast<sockaddr*>(&server_addr), &len);
    if (n < 0) {
        handle_error();
    }
    return n;
}

ssize_t Endpoint::send_bytes(std::span<const std::byte> _buffer, const sockaddr_in& _sockaddr) {
    ssize_t sent = sendto(socket_fd, _buffer.data(), _buffer.size(), 0, reinterpret_cast<const sockaddr*>(&server_addr), sizeof(server_addr));
    if (sent < 0) {
        handle_error();
    }
    return sent;
}

std::vector<std::byte> Endpoint::make_message(MessageType _type) {
    std::vector<std::byte> msg = {static_cast<std::byte>(_type)};
    return msg;
}

std::vector<std::byte> Endpoint::make_message(MessageType _type, std::vector<std::byte>& _payload) {
    std::vector<std::byte> msg;
    msg.reserve(_payload.size() + 1);
    msg.push_back(static_cast<std::byte>(_type));
    msg.insert(msg.end(), _payload.begin(), _payload.end());
    return msg;
}

// Get functions
int Endpoint::get_socket_fd() const {
    return socket_fd;
}

const sockaddr_in& Endpoint::get_server_sockaddr() const {
    return server_addr;
}