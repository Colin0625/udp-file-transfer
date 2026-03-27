#include "Server.hpp"
#include "Endpoint.hpp"

#include <iostream>
#include <vector>

Server::Server() : Endpoint() {}

void Server::start(int _port) {
    port = _port;
    std::cout << "Starting server..." << std::endl;
    initialize_server_addr(_port, INADDR_ANY);
    bind_socket();
    std::cout << "Server started" << std::endl;
}

void Server::receive_message() {
    std::vector<std::byte> buffer(1024);
    sockaddr_in sender_addr;

    ssize_t n = recv_bytes(buffer, sender_addr);
    
    MessageType t = static_cast<MessageType>(buffer[0]);

    if (t == MessageType::CONNECT) {
        std::cout << "Received connection request from " << get_sockaddr_ip(sender_addr) << std::endl;
        handle_conn_message(sender_addr);
        std::cout << "Accepted connection request from " << get_sockaddr_ip(sender_addr) << std::endl;
    }
}


ssize_t Server::handle_conn_message(sockaddr_in& _addr) {
    std::vector<std::byte> msg = make_ok_message();
    return send_bytes(msg, _addr);
}

ssize_t Server::handle_get_message(sockaddr_in& _addr, std::span<std::byte> _buffer) {
    // Placeholder
    std::vector<std::byte> msg = make_ok_message();
    return send_bytes(msg, _addr);
}

ssize_t Server::handle_close_message(sockaddr_in& _addr) {
    std::vector<std::byte> msg = make_ok_message();
    return send_bytes(msg, _addr);
}
