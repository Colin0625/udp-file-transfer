#include <vector>
#include <cstddef>
#include <unistd.h>

#include "transfer/Endpoint.hpp"

Endpoint::Endpoint() : socket_{}, queue_{}, running_(false) {}


void Endpoint::bind_endpoint(const SocketAddress& addr) {
    socket_.bind_socket(addr);
}

void Endpoint::packet_receiver() {
    running_ = true;
    while (running_) {
        std::vector<std::byte> buffer(Packet::max_packet_size);
        SocketAddress addr{};
        ssize_t received = socket_.receive_from(buffer, addr);
        Packet p = Packet::parse(buffer, received, addr);
        queue_.enqueue(p);
    }
}

ssize_t Endpoint::send_packet(const Packet& packet, const SocketAddress& addr) {
    return socket_.send_to(packet.serialize(), addr);
}

int Endpoint::close_receiver() {
    if (!running_) {
        return 1;
    }
    running_ = false;
    return close(socket_.get_socket_fd());
}