#include "net/UdpSocket.hpp"
#include <cstddef>
#include <iostream>

#include "transfer/ServerSession.hpp"


ServerSession::ServerSession() : socket_{}, server_address_{}, file_packager_{}, current_client_address_{} {}

int ServerSession::start_server(uint16_t port) {
    server_address_ = SocketAddress::any(port);
    socket_.bind_socket(server_address_);
    return 0;
}

int ServerSession::accept_connection() {
    SocketAddress client_address_{};
    std::vector<std::byte> buffer(1024);
    Packet synack(MessageType::SYNACK);

    ssize_t received = socket_.receive_from(buffer, client_address_);
    Packet syn = Packet::parse(buffer, received);
    if (syn.get_header().type_ != MessageType::SYN) {
        std::cout << "Recieved non SYN from " << client_address_.get_ip() << std::endl;
        return 1;
    }

    buffer = std::vector<std::byte>(1024);
    ssize_t sent = socket_.send_to(synack.serialize(), client_address_);
    ssize_t response = socket_.receive_from(buffer, client_address_);

    Packet ack = Packet::parse(buffer, response);
    if (ack.get_header().type_ != MessageType::ACK) {
        std::cout << client_address_.get_ip() << " failed to connect" << std::endl;
        return 1;
    }

    std::cout << "Connected to " << client_address_.get_ip() << std::endl;
    current_client_address_ = client_address_;
    return 0;
}

