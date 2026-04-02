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
    
    SocketAddress server_addr = SocketAddress::localhost(5000);

    std::string msg = "Hello from laptop!";
    std::vector<std::byte> buf(reinterpret_cast<std::byte*>(msg.data()), reinterpret_cast<std::byte*>(msg.data()) + msg.size());
    Packet p(MessageType::DATA, 65, buf);

    int sent = sock.send_to(p.serialize(), server_addr);
    std::cout << "Sent " << sent << " bytes to the server" << std::endl;

    return 1;
}