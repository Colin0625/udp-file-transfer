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
    std::vector<std::byte> data = make_message(_t);
    std::span<const std::byte> msg(data);
    send_bytes(msg, get_server_sockaddr());
    std::cout << "Sent message" << std::endl;
}