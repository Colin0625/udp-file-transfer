#pragma once

#include <arpa/inet.h>
#include <string>
#include <span>

class Endpoint {
private:
    int socket_fd;
    sockaddr_in server_addr{};
public:
    Endpoint();
    void handle_error();
    void initialize_sock_addr(int _port, sockaddr_in& _sockaddr, in_addr_t _ipaddr);
    void initialize_sock_addr(int _port, sockaddr_in& _sockaddr, const std::string& _ipaddr);
    void bind_socket(int _fd, const sockaddr_in& _sockaddr);
    std::string get_sockaddr_ip(const sockaddr_in& _sockaddr);
    ssize_t recv_bytes(int _fd, std::span<std::byte>& _buffer, sockaddr_in& _sockaddr);
    ssize_t send_bytes(int _fd, std::span<std::byte>& _buffer, size_t n, const sockaddr_in& _sockaddr);
};
