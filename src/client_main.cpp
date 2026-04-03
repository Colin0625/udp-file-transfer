#include "net/SocketAddress.hpp"
#include "net/UdpSocket.hpp"
#include "protocol/Packet.hpp"
#include "protocol/MessageType.hpp"
#include "transfer/ClientSession.hpp"
#include <string>
#include <cstddef>
#include <vector>
#include <iostream>


int main() {
    ClientSession client{};

    client.connect_to_server(5000, "127.0.0.1");
    

    return 0;
}