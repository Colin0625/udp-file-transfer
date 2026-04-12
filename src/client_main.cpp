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
    Packet p(MessageType::SYN);

    client.send(p, server_addr);


    return 0;
}