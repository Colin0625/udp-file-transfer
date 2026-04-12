#include <vector>
#include <cstddef>
#include <unistd.h>
#include <poll.h>
#include <cerrno>
#include <cstring>

#include "transfer/Endpoint.hpp"

Endpoint::Endpoint() : socket_{}, queue_{}, running_(false), thread_{} {}


void Endpoint::bind_endpoint(const SocketAddress& addr) {
    socket_.bind_socket(addr);
}

void Endpoint::reception_loop() {
    pollfd pfd;
    pfd.fd = socket_.get_socket_fd();
    pfd.events = POLLIN;
    pfd.revents = 0;
    while (running_) {
        std::vector<std::byte> buffer(Packet::max_packet_size);
        SocketAddress addr{};
        int ret = poll(&pfd, 1, 100);
        if (ret > 0 && (pfd.revents & POLLIN)) {
            ssize_t received = socket_.receive_from(buffer, addr);
            Packet p = Packet::parse(buffer, received, addr);
            queue_.enqueue(p);
        }
        else if (ret < 0) {
            throw std::strerror(errno);
        }
    }
}

ssize_t Endpoint::send_packet(const Packet& packet, const SocketAddress& addr) {
    return socket_.send_to(packet.serialize(), addr);
}

void Endpoint::start_receiver() {
    running_ = true;
    thread_ = std::thread(&Endpoint::reception_loop, this);
}

int Endpoint::stop_receiver() {
    if (!running_) {
        return 1;
    }
    running_ = false;
    if (thread_.joinable()) {
        thread_.join();
    }
    return close(socket_.get_socket_fd());
}

Packet* Endpoint::get_front_packet() {
    return queue_.dequeue();
}