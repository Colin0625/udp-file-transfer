#include <string>
#include <cstddef>
#include <vector>
#include <iostream>

#include "net/SocketAddress.hpp"
#include "net/UdpSocket.hpp"
#include "protocol/Packet.hpp"
#include "protocol/MessageType.hpp"
#include "transfer/ClientSession.hpp"

int main() {
    ClientSession client{};
    SocketAddress server_addr = SocketAddress::localhost(5000);
    Packet p(MessageType::METADATA);

    client.start_listening();
    std::cout << "Started listening" << std::endl;
    
    client.send(p, server_addr);
    std::cout << "sent packet" << std::endl;

    client.stop_listening();
    std::cout << "stopped listening" << std::endl;
    return 0;
}