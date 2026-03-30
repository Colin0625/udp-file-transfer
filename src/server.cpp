#include "SocketAddress.hpp"
#include "UdpSocket.hpp"
#include <vector>
#include <cstddef>
#include <iostream>

int main() {
    UdpSocket sock = UdpSocket();
    SocketAddress addr = SocketAddress::any(5000);
    sock.bind_socket(addr);
    

    std::vector<std::byte> buffer(1024);
    SocketAddress sender_addr{};
    sock.receive_from(buffer, sender_addr);
    std::cout << "Received message" << std::endl;
    return 1;
}