#include "net/SocketAddress.hpp"
#include "net/UdpSocket.hpp"
#include "protocol/Packet.hpp"
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

    std::span<std::byte> sp(buffer.begin(), buffer.begin() + n);

    Packet p = Packet::parse(sp);
    for (std::byte b : p.get_payload()) {
        std::cout << static_cast<char>(b);
    }
    std::cout << std::endl;
    return 1;
}