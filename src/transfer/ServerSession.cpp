#include <thread>

#include "transfer/ServerSession.hpp"

ServerSession::ServerSession() : endpoint_{}, server_address_{} {}

void ServerSession::bind_server(int port) {
    SocketAddress addr = SocketAddress::any(port);
    endpoint_.bind_endpoint(addr);
}

void ServerSession::start() {
    endpoint_.start_receiver();
}

void ServerSession::stop() {
    endpoint_.stop_receiver();
}

Packet* ServerSession::front() {
    return endpoint_.get_front_packet();
}