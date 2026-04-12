#include <thread>

#include "transfer/ClientSession.hpp"
#include "utility/Queue.hpp"
#include "protocol/Packet.hpp"

ClientSession::ClientSession() : endpoint_{}, server_address_{} {}

void ClientSession::start() {
    endpoint_.start_receiver();
}

void ClientSession::stop() {
    endpoint_.stop_receiver();
}

ssize_t ClientSession::send(Packet p, SocketAddress addr) {
    return endpoint_.send_packet(p, addr);
}

Packet* ClientSession::front() {
    return endpoint_.get_front_packet();
}