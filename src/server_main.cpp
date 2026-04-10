#include <vector>
#include <cstddef>
#include <iostream>

#include "net/SocketAddress.hpp"
#include "net/UdpSocket.hpp"
#include "protocol/Packet.hpp"
#include "transfer/ServerSession.hpp"

int main() {
    ServerSession server{};

    server.start_server(5000);
    server.accept_connection();

    return 0;
}