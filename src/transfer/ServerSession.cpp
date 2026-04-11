#include <thread>

#include "transfer/ServerSession.hpp"

ServerSession::ServerSession() : endpoint_{}, server_address_{} {}

void ServerSession::bind_server(int port) {
    SocketAddress addr = SocketAddress::any(port);
    endpoint_.bind_endpoint(addr);
}

std::thread ServerSession::start_listening() {
    return std::thread(&Endpoint::packet_receiver, &this->endpoint_);
}

int ServerSession::stop_listening() {
    return endpoint_.close_receiver();
}