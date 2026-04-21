#include <thread>

#include "transfer/ServerSession.hpp"

ServerSession::ServerSession()
 : endpoint_{}, server_address_{}, state_(SessionState::OFF) {}

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

Packet ServerSession::front() {
    return endpoint_.get_front_packet();
}

void ServerSession::handle_packet() {
    Packet packet = endpoint_.get_front_packet();
    MessageType mt = packet.get_header().type_;
    
}