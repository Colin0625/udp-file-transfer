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

    Packet p = Packet::parse(sp, n);
    for (std::byte b : p.get_payload()) {
        std::cout << static_cast<char>(b);
    }
    std::cout << std::endl;

    std::string msg = "Thanks for the message, this is the server.";
    std::vector<std::byte> buf(reinterpret_cast<std::byte*>(msg.data()), reinterpret_cast<std::byte*>(msg.data()) + msg.size());
    Packet p(MessageType::DATA, 67, buf);

    int sent = sock.send_to(p.serialize(), sender_addr);
    std::cout << "Sent " << sent << " bytes to the client" << std::endl;

    return 1;
}