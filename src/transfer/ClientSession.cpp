#include <thread>

#include "transfer/ClientSession.hpp"
#include "utility/Queue.hpp"
#include "protocol/Packet.hpp"

ClientSession::ClientSession() : endpoint_{}, server_address_{} {}

void ClientSession::start_listening() {
    
    std::thread t = std::thread(&Endpoint::packet_receiver, &this->endpoint_);
    thread_ = std::move(t);
}

int ClientSession::stop_listening() {
    int res = endpoint_.close_receiver();
    std::cout << "result: " << res << std::endl;
    thread_.join();
    return res;
}

Queue<Packet>* ClientSession::get_queue() {
    return endpoint_.get();
}

ssize_t ClientSession::send(Packet p, SocketAddress addr) {
    return endpoint_.send_packet(p, addr);
}