#pragma once

#include "Protocol.hpp"
#include <arpa/inet.h>
#include <string>
#include <span>
#include <vector>

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
    ssize_t recv_bytes(std::span<std::byte> _buffer, sockaddr_in& _sockaddr);
    ssize_t send_bytes(std::span<const std::byte> _buffer, const sockaddr_in& _sockaddr);
    std::vector<std::byte> make_message(MessageType _type);
    std::vector<std::byte> make_message(MessageType _type, std::span<const std::byte> _payload);
    std::vector<std::byte> make_conn_message();
    std::vector<std::byte> make_ok_message();
    std::vector<std::byte> make_get_message(const std::string& _filename);
    std::vector<std::byte> make_size_message(uint16_t _numpackets, uint32_t _filesize, const std::string& _filename);
    std::vector<std::byte> make_data_message(uint16_t _seqpacket,  std::span<const std::byte> _payload);
    std::vector<std::byte> make_close_message();
    std::vector<std::byte> make_error_message(std::string _error);
    uint32_t checksum(std::span<const std::byte> _data);

    // Get functions
    int get_socket_fd() const;
    const sockaddr_in& get_server_sockaddr() const;
};
