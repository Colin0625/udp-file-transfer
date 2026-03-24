#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cerrno>
#include <string.h>
#include <string>
#include <unistd.h>
#include <span>


int PORT = 5000;

void handle_error() {
    throw std::runtime_error(strerror(errno));
}

std::string get_sockaddr_ip(const sockaddr_in& addr) {
    char addr_ip[INET_ADDRSTRLEN];
    if(inet_ntop(AF_INET, &addr.sin_addr, addr_ip, sizeof(addr_ip)) == nullptr) {
        handle_error();
    }
    return addr_ip;
}

int create_socket(int domain, int type, int protocol) {
    int socket_fd = socket(domain, type, protocol);
    if (socket_fd < 0) {
        handle_error();
    }
    std::cout << "Socket created successfully" << std::endl;
    return socket_fd;
}

sockaddr_in initialize_server_address(int port, int domain, const std::string address) {
    sockaddr_in serv_addr{};
    serv_addr.sin_port = htons(port);
    serv_addr.sin_family = domain;
    int p = inet_pton(domain, address.c_str(), &serv_addr.sin_addr);
    if (p < 0) {
        handle_error();
    }
    else if (!p) {
        throw std::runtime_error("Invalid IP address");
    }
    return serv_addr;
}

ssize_t send_message(int fd, const std::string& message, const sockaddr_in& addr) {
    ssize_t sent = sendto(fd, message.c_str(), message.length(), 0, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr));
    if (sent < 0) {
        handle_error();
    }
    std::cout << "Me: " << message << std::endl;
    return sent;
}

void display_message(std::span<char>& buffer, const sockaddr_in& client_addr) {
    std::string client_ip = get_sockaddr_ip(client_addr);
    std::cout << client_ip << ": " << buffer.data() << std::endl;
}

ssize_t wait_for_message(int fd, std::span<char>& buffer, sockaddr_in& addr) {
    socklen_t len = sizeof(addr);
    std::cout << "Waiting for message..." << std::endl;
    ssize_t n = recvfrom(fd, buffer.data(), buffer.size() - 1, 0, reinterpret_cast<sockaddr*>(&addr), &len);
    if (n < 0) {
        handle_error();
    }
    buffer[n] = '\0';
    return n;
}

std::string get_message() {
    std::string msg;
    std::cout << "Enter message >>> ";
    std::getline(std::cin, msg);
    return msg;
}

void communication_loop(int fd, sockaddr_in addr) {
    while (true) {
        std::string msg = get_message();
        ssize_t sent = send_message(fd, msg, addr);
        
        char raw[256];
        std::span<char> buffer(raw);
        ssize_t n = wait_for_message(fd, buffer, addr);
        display_message(buffer, addr);
    }
}

int main() {

    // std::string ip = "127.0.0.1"; // localhost
    std::string ip = "100.83.112.113"; // tailscale
    int socket_fd;
    sockaddr_in serv_addr;

    socket_fd = create_socket(AF_INET, SOCK_DGRAM, 0);

    serv_addr = initialize_server_address(PORT, AF_INET, ip);

    communication_loop(socket_fd, serv_addr);
    
    close(socket_fd);
    return 0;
}