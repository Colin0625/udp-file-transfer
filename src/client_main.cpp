#include <string>
#include <cstddef>
#include <vector>
#include <iostream>
#include <filesystem>

#include "net/SocketAddress.hpp"
#include "net/UdpSocket.hpp"
#include "protocol/Packet.hpp"
#include "protocol/MessageType.hpp"
#include "transfer/ClientSession.hpp"

int main() {
    ClientSession client{};
    // SocketAddress server_addr = SocketAddress::localhost(5000);
    SocketAddress server_addr(5000, "100.110.209.74"); // desktop as server
    client.set_server_address(server_addr);
    client.start();

    client.connect_to_server();
    client.request_file(std::filesystem::path("test-files/cpp.png"));
    client.manage_packet();

    client.stop();
    return 0;
}
