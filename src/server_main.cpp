#include <vector>
#include <cstddef>
#include <iostream>
#include <thread>
#include <chrono>

#include "net/SocketAddress.hpp"
#include "net/UdpSocket.hpp"
#include "protocol/Packet.hpp"
#include "transfer/ServerSession.hpp"

int main() {
    ServerSession server{};

    server.bind_server(5000);
    server.start();
    
    std::cout << "Starting to listen" << std::endl;
    while (true) {
        Packet p = server.front();
        p.print();
    }
    server.stop();

    


    return 0;
}