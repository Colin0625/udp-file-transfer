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


// test-file/matthew.jpg
// test-file/cpp.png

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Must pass requested file" << std::endl;
        return 0;
    }
    ClientSession client{};
    client.verbose = false;
    // SocketAddress server_addr = SocketAddress::localhost(5000);
    // SocketAddress server_addr(5000, "100.110.209.74"); // desktop as server
    SocketAddress server_addr(5000, "10.105.101.219");
    client.set_server_address(server_addr);
    client.start();

    client.connect_to_server();
    client.request_file(std::filesystem::path(argv[1]));
    client.manage_packet();

    client.stop();
    return 0;
}
