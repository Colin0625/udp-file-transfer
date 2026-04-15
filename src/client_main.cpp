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
    

    int s;
    std::cout << "Press enter to send a packet >>> ";
    std::cin >> s;
    for (int i = 0; i < 5; i++) {
        Packet p(static_cast<MessageType>(s));
        std::cout << "Sending packet" << std::endl;
        ssize_t sent = client.send(p, server_addr);
        std::cout << "Sent " << sent << " bytes to server." << std::endl;
    }


    return 0;
}