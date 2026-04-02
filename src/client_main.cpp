#include "net/SocketAddress.hpp"
#include "net/UdpSocket.hpp"
#include "protocol/Packet.hpp"
#include "protocol/MessageType.hpp"
#include <string>
#include <cstddef>
#include <vector>
#include <iostream>


int main() {
    UdpSocket sock = UdpSocket();
    
    SocketAddress server_addr(5000, "100.83.112.113");

    std::string msg = "Hello from desktop!\nThis is a test message to check the UDP communication between the client and server.";
    std::vector<std::byte> buf(reinterpret_cast<std::byte*>(msg.data()), reinterpret_cast<std::byte*>(msg.data()) + msg.size());
    Packet p(MessageType::DATA, 65, buf);

    int sent = sock.send_to(p.serialize(), server_addr);
    std::cout << "Sent " << sent << " bytes to the server" << std::endl;
    std::vector<std::byte> re(1024);
    SocketAddress sender_addr{};
    int n = sock.receive_from(re, sender_addr);

    Packet p = Packet::parse(re, n);

    return 1;
}