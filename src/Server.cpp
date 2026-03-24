#include "Server.hpp"
#include "Endpoint.hpp"
#include <iostream>
#include <vector>

int main() {
    std::cout << "Initializing server" << std::endl;
    Endpoint server;
    server.initialize_server_addr(5000, INADDR_ANY);
    server.bind_socket();

    std::cout << "Waiting for message..." << std::endl;
    sockaddr_in client_addr{};
    std::vector<std::byte> storage(1024);
    std::span<std::byte> buffer(storage);
    ssize_t n = server.recv_bytes(buffer, client_addr);

    std::span<std::byte> received(storage.data(), n);

    std::string message(reinterpret_cast<const char*>(received.data()), received.size());
    std::cout << message << std::endl;
}