#include "Client.hpp"
#include "Endpoint.hpp"
#include <iostream>
#include <vector>

int main() {
    std::cout << "Initializing client" << std::endl;
    std::string ip = "127.0.0.1";

    Endpoint client;
    client.initialize_server_addr(5000, ip);

    std::string msg = "Hello from client!";
    std::span<const std::byte> buffer(reinterpret_cast<const std::byte*>(msg.data()), msg.size());

    client.send_bytes(buffer, client.get_server_sockaddr());
    std::cout << "Sent message" << std::endl;
}