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
}