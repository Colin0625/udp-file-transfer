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
    // SocketAddress server_addr = SocketAddress::localhost(5000);
    SocketAddress server_addr(5000, "100.83.112.113");
    client.start();


    client.stop();
    return 0;
}