#include "SocketAddress.hpp"
#include "UdpSocket.hpp"
#include <string>
#include <cstddef>
#include <vector>
#include <iostream>


int main() {
    UdpSocket sock = UdpSocket();
    
    SocketAddress server_addr = SocketAddress::localhost(5000);

    std::string msg = "Hello from localhost!";
    std::byte* msgptr = reinterpret_cast<std::byte*>(msg.data());

    std::vector<std::byte> payload(msgptr, msgptr + msg.size());
    sock.send_to(payload, server_addr);
    std::cout << "Sent message to server" << std::endl;

    return 1;
}