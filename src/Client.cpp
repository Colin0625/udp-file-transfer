#include "Client.hpp"
#include "Endpoint.hpp"
#include "Protocol.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>

Client::Client() : Endpoint() {
    std::cout << "Client started" << std::endl;
}

int Client::connect_to_server(int _port, std::string _ip) {
    std::cout << "Connecting to server..." << std::endl;
    initialize_server_addr(_port, _ip);
    std::vector<std::byte> msg = make_conn_message();
    ssize_t sent = send_bytes(msg, get_server_sockaddr());

    std::vector<std::byte> buffer(1024);
    sockaddr_in sender_addr;

    ssize_t n = recv_bytes(buffer, sender_addr);

    if (buffer[0] == static_cast<std::byte>(MessageType::OK)) {
        std::cout << "Connected to server at " << _ip << " on port " << _port << std::endl;
        return 1;
    }
    return -1;
}

int Client::request_file(std::string& _filename) {
    std::vector<std::byte> msg = make_get_message(_filename);
    std::vector<std::byte> buffer(1024);
    sockaddr_in sender_addr;

    ssize_t sent = send_bytes(msg, get_server_sockaddr());

    ssize_t n = recv_bytes(buffer, sender_addr);
    std::cout << "Requested " << _filename << std::endl;
    if (buffer[0] == static_cast<std::byte>(MessageType::OK)) {
        std::cout << "Server accepted transfer of " << _filename << std::endl;
        if (receive_file_size(sender_addr) < 0) {
            std::cout << "Reception failed" << std::endl;
            return -1;
        }
        std::cout << "Reception of " << _filename << " completed" << std::endl;
        return 1;
    }
    else {
        std::cout << "Server denied transfer of " << _filename << std::endl;
        return -1;
    }
}

int Client::receive_file_size(sockaddr_in& _addr) {
    std::cout << "Ready to receive message" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::vector<std::byte> msg = make_ok_message();
    std::vector<std::byte> buffer(1024);
    sockaddr_in sender_addr;

    ssize_t sent = send_bytes(msg, _addr);

    ssize_t n = recv_bytes(buffer, sender_addr);
    if (buffer[0] == static_cast<std::byte>(MessageType::SIZE)) {
        uint16_t num_packets;
        uint32_t file_size;
        uint8_t fname_size;
        std::string filename;

        num_packets = (static_cast<uint16_t>(buffer[1])) | (static_cast<uint16_t>(buffer[2]) << 8);
        file_size = (static_cast<uint32_t>(buffer[3])) | 
                    (static_cast<uint32_t>(buffer[4]) << 8 ) | 
                    (static_cast<uint32_t>(buffer[5]) << 16) | 
                    (static_cast<uint32_t>(buffer[6]) << 24);
        fname_size = static_cast<uint8_t>(buffer[7]);
        const char* nameptr = reinterpret_cast<const char*>(&buffer[8]);
        filename = std::string(nameptr, nameptr + fname_size);
        receive_file_data(sender_addr, num_packets, file_size, filename);
    }
    else {
        std::cout << "File transfer failed" << std::endl;
        return -1;
    }
    return 1;
}

int Client::receive_file_data(sockaddr_in& _addr, int _numpackets, int _filesize, std::string& _filename) {
    std::vector<std::byte> msg = make_ok_message();
    std::vector<std::byte> buffer(1024);
    std::vector<std::vector<std::byte>> file_vec(_numpackets);
    sockaddr_in sender_addr;

    std::cout << "_filename: " << _filename << std::endl;
    std::ofstream out(_filename, std::ios::binary);

    ssize_t sent = send_bytes(msg, _addr);

    for (int i = 0; i < _numpackets; i++) {
        std::cout << "Receiving packet " << i << std::endl;
        ssize_t n = recv_bytes(buffer, sender_addr);
        if (buffer[0] != static_cast<std::byte>(MessageType::DATA)) {
            std::cout << "Error during transmission" << std::endl;
            return -1;
        }

        uint16_t seq = (static_cast<uint16_t>(buffer[1])) | (static_cast<uint16_t>(buffer[2]) << 8);
        uint16_t psize = (static_cast<uint16_t>(buffer[3])) | (static_cast<uint16_t>(buffer[4]) << 8);
        uint32_t csum = (static_cast<uint32_t>(buffer[5])) |
                        (static_cast<uint32_t>(buffer[6])) |
                        (static_cast<uint32_t>(buffer[7])) |
                        (static_cast<uint32_t>(buffer[8]));
        std::span<std::byte> payload(buffer.begin() + 9, buffer.begin() + 9 + psize);
        file_vec[seq] = std::vector<std::byte>(payload.begin(), payload.end());
    }

    for (auto i : file_vec) {
        if (i.empty()) {
            std::cout << "Failure to transmit all packets" << std::endl;
            return -1;
        }
    }

    for (auto i : file_vec) {
        out.write(reinterpret_cast<const char*>(i.data()), i.size());
    }
    out.close();
    std::cout << "File transfer completed" << std::endl;
    return 1;
}