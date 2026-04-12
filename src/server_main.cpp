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
    
    std::this_thread::sleep_for(std::chrono::seconds(2));

    Packet* p = server.front();
    if (p != nullptr) {
        p->print();
    }
    server.stop();

    return 0;
}