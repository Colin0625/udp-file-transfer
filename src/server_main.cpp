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
    Queue<Packet>* q = server.get_queue();

    server.start_listening();
    std::cout << "started listening" << std::endl;
    
    std::cout << "Starting sleep" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    std::cout << "done sleeping" << std::endl;

    q->dequeue()->print();
    std::cout << "printed" << std::endl;

    server.stop_listening();
    std::cout << "stopped listening" << std::endl;


    return 0;
}