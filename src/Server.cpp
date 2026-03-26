#include "Server.hpp"
#include "Endpoint.hpp"

#include <iostream>
#include <vector>

Server::Server(int _port) : Endpoint(), port(_port) {

}

void Server::start() {
    std::cout << "Starting server..." << std::endl;
    initialize_server_addr(port, INADDR_ANY);
    bind_socket();
    std::cout << "Server started" << std::endl;
}

void Server::receive_message() {
    std::vector<std::byte> buffer(1024);
    ssize_t n = recv_bytes(buffer, client_addr);
    std::cout << static_cast<int>(buffer[0]) << std::endl;
    std::span<std::byte> msg(buffer.begin() + 1, buffer.begin() + n);
    std::string s(reinterpret_cast<const char*>(msg.data()), msg.size());
    std::span<const std::byte> sp = std::as_bytes(std::span{s});
    std::cout << s << std::endl;
    for (char c : s) {
        std::cout << c << std::endl;
    }
    std::cout << checksum(sp) << std::endl;
    
}

