#include <vector>
#include <cstddef>
#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>
#include <atomic>

#include "net/SocketAddress.hpp"
#include "net/UdpSocket.hpp"
#include "protocol/Packet.hpp"
#include "transfer/ServerSession.hpp"






int main() {
    ServerSession server{};
    std::cout << sizeof(sockaddr_in) << " " << sizeof(std::string) << std::endl;



    server.bind_server(5000);
    server.start();
    
    while (true) {
        server.manage_packet();
    }

    server.stop();

    


    return 0;
}