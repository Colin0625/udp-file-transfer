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
    UdpSocket socket{};


    SocketAddress server_addr = SocketAddress::localhost(5000);

    std::vector<std::byte> vec = {std::byte(0xfe), std::byte(0xef)};

    Packet p(MessageType::ERROR, server_addr, vec);

    std::cout << "Press enter to send data to server" << std::flush;
    std::cin.get();

    ssize_t sent = socket.send_to(p.serialize(), server_addr);
    std::cout << "Sent " << sent << " bytes to the server" << std::endl;



    return 0;
}