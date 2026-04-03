#include "transfer/ClientSession.hpp"
#include <string>
#include <iostream>

ClientSession::ClientSession()
 : socket_{}, server_address_{}, file_reassembler_{}
{}


void ClientSession::connect_to_server(uint16_t port, const std::string& ip_addr) {
    server_address_ = SocketAddress(port, ip_addr);
    Packet syn(MessageType::SYN);
    std::vector<std::byte> buffer(1024);
    SocketAddress sender_addr;

    ssize_t sent = socket_.send_to(syn.serialize(), server_address_);
    ssize_t received = socket_.receive_from(buffer, sender_addr);
    
    Packet synack = Packet::parse(buffer, received);
    if (synack.get_header().type_ != MessageType::SYNACK) {
        std::cout << "Failed to connect to " << sender_addr.get_ip() << std::endl;
        return;
    }

    Packet ack(MessageType::ACK);
    
    ssize_t response = socket_.send_to(ack.serialize(), server_address_);
    std::cout << "Connected to server " << server_address_.get_ip() << std::endl;
}