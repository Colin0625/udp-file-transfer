#include "Server.hpp"
#include "Endpoint.hpp"

#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <thread>

Server::Server() : Endpoint() {}

void Server::start(int _port) {
    port = _port;
    std::cout << "Starting server..." << std::endl;
    initialize_server_addr(_port, INADDR_ANY);
    bind_socket();
    std::cout << "Server started" << std::endl;
}

void Server::receive_message() {
    std::vector<std::byte> buffer(1024);
    sockaddr_in sender_addr;

    ssize_t n = recv_bytes(buffer, sender_addr);
    
    MessageType t = static_cast<MessageType>(buffer[0]);

    if (t == MessageType::CONNECT) {
        std::cout << "Received connection request from " << get_sockaddr_ip(sender_addr) << std::endl;
        handle_conn_message(sender_addr);
        std::cout << "Accepted connection request from " << get_sockaddr_ip(sender_addr) << std::endl;
    }
    else if (t == MessageType::GET) {
        std::cout << "Received get request from " << get_sockaddr_ip(sender_addr) << std::endl;
        handle_get_message(sender_addr, buffer);
    }
}


ssize_t Server::handle_conn_message(sockaddr_in& _addr) {
    std::vector<std::byte> msg = make_ok_message();
    return send_bytes(msg, _addr);
}

ssize_t Server::handle_get_message(sockaddr_in& _addr, std::span<std::byte> _buffer) {
    uint8_t fname_size = static_cast<uint8_t>(_buffer[1]);
    std::cout << "fname_size: " << (int)fname_size << std::endl;
    const char* fnameptr = reinterpret_cast<const char*>(&_buffer[2]);
    std::string filename(fnameptr, fnameptr + fname_size);





    std::cout << "Checking if " << filename << ", exists." << std::endl;
    if (std::filesystem::exists(std::string("files/") + filename)) {
        std::cout << filename << ", exists" << std::endl;
        std::vector<std::byte> msg = make_ok_message();
        std::vector<std::byte> buff(1024);
        sockaddr_in sender_addr;
        
        ssize_t sent = send_bytes(msg, _addr);
        ssize_t n = recv_bytes(buff, sender_addr);

        if (buff[0] == static_cast<std::byte>(MessageType::OK)) {
            ssize_t sent = send_file(sender_addr, filename);
        }
        else {
            return -1;
        }
        
    }
    else {
        std::cout << filename << " does not exist" << std::endl;
        std::vector<std::byte> msg = make_error_message("File does not exist");
        ssize_t sent = send_bytes(msg, _addr);
        return -1;
    }
    return 1;
}

ssize_t Server::handle_close_message(sockaddr_in& _addr) {
    std::vector<std::byte> msg = make_ok_message();
    return send_bytes(msg, _addr);
}

ssize_t Server::send_file(sockaddr_in& _addr, std::string& _filename) {
    std::filesystem::path file_path = std::filesystem::path("files") / _filename;

    std::ifstream file(file_path, std::ios::binary);


    if (!file) {
        throw std::runtime_error("Failed to open file");
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);


    uint32_t fsize = static_cast<uint32_t>(size);
    uint16_t num_packets = fsize/1015;
    if (fsize % 1015) {num_packets++;}
    
    std::vector<std::byte> msg = make_size_message(num_packets, fsize, _filename);
    std::vector<std::byte> buffer(1024);
    sockaddr_in sender_addr;


    ssize_t sent = send_bytes(msg, _addr);
    ssize_t n = recv_bytes(buffer, sender_addr);

    if (buffer[0] == static_cast<std::byte>(MessageType::OK)) {

        const size_t chunk_size = 1015;
        std::vector<std::byte> buffer(chunk_size);

        int seq = 0;
        std::cout << "Starting file transfer" << std::endl;
        std::cout << "Number of packets: " << num_packets << std::endl;
        std::cout << "Total size of file: " << fsize << std::endl;
        while (file.read(reinterpret_cast<char*>(buffer.data()), chunk_size) || file.gcount() > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            size_t bytes_read = file.gcount();
            std::vector<std::byte> msg = make_data_message(seq, buffer);
            ssize_t sent = send_bytes(msg, _addr);
            seq++;
        }
        std::cout << "Finished sending files" << std::endl;
        return 1;
    }
    else {
        std::cout << "Client did not respond to size message" << std::endl;
        return -1;
    }
    
}