#include <vector>
#include <cstddef>
#include <unistd.h>

#include "transfer/Endpoint.hpp"

Endpoint::Endpoint() : socket_{}, queue_{}, running_(false), thread_{} {}


void Endpoint::bind_endpoint(const SocketAddress& addr) {
    socket_.bind_socket(addr);
}

void Endpoint::reception_loop() {
    while (running_) {
        std::vector<std::byte> buffer(Packet::max_packet_size);
        SocketAddress addr{};
        ssize_t received{};
        try {
            received = socket_.receive_from(buffer, addr);
        }
        catch (const std::exception& e) {
            if (!running_) {
                return;
            }
            throw;
        }
        Packet p = Packet::parse(buffer, received, addr);
        queue_.enqueue(p);
    }
}

ssize_t Endpoint::send_packet(const Packet& packet, const SocketAddress& addr) {
    return socket_.send_to(packet.serialize(), addr);
}

void Endpoint::start_receiver() {
    running_ = true;
    thread_ = std::move(std::thread(Endpoint::reception_loop, this));
}

int Endpoint::stop_receiver() {
    if (!running_) {
        return 1;
    }
    running_ = false;
    return close(socket_.get_socket_fd());
}

Packet* Endpoint::get_front_packet() {
    return queue_.dequeue();
}