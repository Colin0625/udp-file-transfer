#include <string>
#include <iostream>
#include <vector>
#include <cstddef>
#include <thread>

#include "transfer/ClientSession.hpp"

ClientSession::ClientSession()
 : socket_{}, server_address_{}, file_reassembler_{}
{}


int ClientSession::connect_to_server(uint16_t port, const std::string& ip_addr) {
    server_address_ = SocketAddress(port, ip_addr);
    Packet syn(MessageType::SYN);
    std::vector<std::byte> buffer(1024);
    SocketAddress sender_addr;

    ssize_t sent = socket_.send_to(syn.serialize(), server_address_);
    ssize_t received = socket_.receive_from(buffer, sender_addr);
    
    Packet synack = Packet::parse(buffer, received);
    if (synack.get_header().type_ != MessageType::SYNACK) {
        std::cout << "Failed to connect to " << sender_addr.get_ip() << std::endl;
        return 1;
    }

    Packet ack(MessageType::ACK);
    
    ssize_t response = socket_.send_to(ack.serialize(), server_address_);
    std::cout << "Connected to server " << server_address_.get_ip() << std::endl;
    return 0;
}


int ClientSession::request_file(const std::string& filename) {
    std::span<std::byte> payload(reinterpret_cast<std::byte*>(const_cast<char*>(filename.data())), filename.size());
    PacketHeader get_header(MessageType::GET, payload.size(), 0, 0);
    Packet get_packet(get_header, payload);
    std::vector<std::byte> buffer(1024);
    SocketAddress sender_address;

    ssize_t sent = socket_.send_to(get_packet.serialize(), server_address_);
    ssize_t received = socket_.receive_from(buffer, sender_address);

    Packet metadata_packet = Packet::parse(buffer, received);

    if (metadata_packet.get_header().type_ != MessageType::METADATA) {
        std::cout << "Server refused transmission of " << filename << std::endl;
        return 1;
    }
    
    std::vector<std::byte> metadata = metadata_packet.get_payload();

    uint64_t filesize = ClientSession::metadata_to_uint64(metadata);

    file_reassembler_.open(filename, filesize);
}

int ClientSession::receive_file() {
    Packet ack_packet(MessageType::ACK);
    std::vector<std::byte> buffer(1024);
    ssize_t sent = socket_.send_to(ack_packet.serialize(), server_address_);
    
}







uint64_t ClientSession::metadata_to_uint64(const std::span<std::byte> metadata) {
    uint64_t filesize = (static_cast<uint64_t>(metadata[0]) << 24) |
                        (static_cast<uint64_t>(metadata[1]) << 16) |
                        (static_cast<uint64_t>(metadata[2]) << 8) |
                        (static_cast<uint64_t>(metadata[3]));
    return filesize;
}

