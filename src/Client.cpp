#include "Client.hpp"
#include "Endpoint.hpp"
#include "Protocol.hpp"
#include <iostream>
#include <vector>

Client::Client(int _port, std::string _ip) : Endpoint(), port(_port), ip(_ip) {
    initialize_server_addr(_port, _ip);
    std::cout << "Initialized client" << std::endl;
}

void Client::send_message(MessageType _t) {
    std::string name = "test.jpg";
    for (char c : name) {
        std::cout << c << std::endl;
    }
    std::vector<std::byte> data = make_error_message(name);
    std::span<const std::byte> msg(data);
    ssize_t sent = send_bytes(msg, get_server_sockaddr());
    std::cout << "Sent message" << std::endl;
    std::span<const std::byte> sp = std::as_bytes(std::span{name});
    std::cout << checksum(sp) << std::endl;
}