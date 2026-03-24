#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cerrno>
#include <string.h>
#include <unistd.h>
#include <string>
#include <span>

int PORT = 5000;

void handle_error() {
    throw std::runtime_error(strerror(errno));
}

int create_socket(int domain, int type, int protocol) {
    int socket_fd = socket(domain, type, protocol);
    if (socket_fd < 0) {
        handle_error();
    }
    std::cout << "Socket created successfully" << std::endl;
    return socket_fd;
}

sockaddr_in initialize_server_address(int port, int domain, in_addr_t address) {
    sockaddr_in serv_addr{};
    serv_addr.sin_port = htons(port);
    serv_addr.sin_family = domain;
    serv_addr.sin_addr.s_addr = address;
    return serv_addr;
}

sockaddr_in initialize_server_address(int port, int domain, std::string address) {
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

void bind_socket(int fd, const sockaddr_in& addr) {
    if (bind(fd, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) < 0) {
        handle_error();
    }
    std::cout << "Bind completed" << std::endl;
}

void initialize_server(int domain, int type, int protocol, int port, int& fd, sockaddr_in& serv_addr, in_addr_t address) {
    fd = create_socket(domain, type, protocol);
    serv_addr = initialize_server_address(port, domain, address);
    bind_socket(fd, serv_addr);
}

ssize_t wait_for_message(int fd, std::span<char>& buffer, sockaddr_in& client_address) {
    socklen_t len = sizeof(client_address);
    std::cout << "Waiting for message..." << std::endl;
    ssize_t n = recvfrom(fd, buffer.data(), buffer.size() - 1, 0, reinterpret_cast<sockaddr*>(&client_address), &len);
    if (n < 0) {
        handle_error();
    }
    buffer[n] = '\0';
    return n;
}

std::string get_sockaddr_ip(const sockaddr_in& addr) {
    char addr_ip[INET_ADDRSTRLEN];
    if(inet_ntop(AF_INET, &addr.sin_addr, addr_ip, sizeof(addr_ip)) == nullptr) {
        handle_error();
    }
    return addr_ip;
}

void display_message(std::span<char>& buffer, const sockaddr_in& client_addr) {
    std::string client_ip = get_sockaddr_ip(client_addr);
    std::cout << client_ip << ": " << buffer.data() << std::endl;
}

std::string get_message() {
    std::string msg;
    std::cout << "Enter message >>> ";
    std::getline(std::cin, msg);
    return msg;
}

ssize_t send_message(int fd, std::string& buffer, size_t n, const sockaddr_in& dest_addr) {
    ssize_t sent = sendto(fd, buffer.c_str(), n, 0, reinterpret_cast<const sockaddr*>(&dest_addr), sizeof(dest_addr));
    std::cout << "Me: " << buffer << std::endl;
    return sent;
}

void communication_loop(int fd, sockaddr_in addr) {
    while (true) {
        char raw[256];
        std::span<char> buffer(raw);
        ssize_t n = wait_for_message(fd, buffer, addr);
        display_message(buffer, addr);

        std::string msg = get_message();
        ssize_t sent = send_message(fd, msg, msg.length(), addr);
    }
}



int main() {

    int socket_fd;
    sockaddr_in serv_addr{}, client_addr{};
    initialize_server(AF_INET, SOCK_DGRAM, 0, PORT, socket_fd, serv_addr, INADDR_ANY);

    communication_loop(socket_fd, client_addr);

    close(socket_fd);
    return 0;
}