#include "net/SocketAddress.hpp"
#include "net/UdpSocket.hpp"
#include <vector>
#include <cstddef>
#include <iostream>

int main() {
    UdpSocket sock = UdpSocket();
    SocketAddress addr = SocketAddress::any(5000);
    sock.bind_socket(addr);
    

    std::vector<std::byte> buffer(1024);
    SocketAddress sender_addr{};
    ssize_t n = sock.receive_from(buffer, sender_addr);
    std::cout << "Received " << n << " bytes: " << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << static_cast<char>(buffer[i]);
    }
    std::cout << std::endl;
    return 1;
}