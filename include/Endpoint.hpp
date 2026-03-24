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
    void initialize_server_addr(int _port, in_addr_t _ipaddr);
    void initialize_server_addr(int _port, const std::string& _ipaddr);
    void bind_socket();
    std::string get_sockaddr_ip(const sockaddr_in& _sockaddr);
    ssize_t recv_bytes(std::span<std::byte>& _buffer, sockaddr_in& _sockaddr);
    ssize_t send_bytes(std::span<const std::byte>& _buffer, const sockaddr_in& _sockaddr);
    // Get functions
    int get_socket_fd() const;
    const sockaddr_in& get_server_sockaddr() const;
};
