#include <vector>
#include <cstddef>
#include <iostream>

#include "net/SocketAddress.hpp"
#include "net/UdpSocket.hpp"
#include "protocol/Packet.hpp"
#include "transfer/ServerSession.hpp"

int main() {
    UdpSocket sock{};
    SocketAddress addr = SocketAddress::any(5000);

    sock.bind_socket(addr);


    std::vector<std::byte> vec(100);


    std::cout << "Press enter to accept incoming data." << std::flush;
    std::cin.get();

    SocketAddress sender_addr{};

    ssize_t n = sock.receive_from(vec, sender_addr);
    std::cout << "received " << n << " bytes from " << sender_addr.get_ip() << std::endl;



    return 0;
}