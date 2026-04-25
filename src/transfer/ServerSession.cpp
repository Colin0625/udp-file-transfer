#include <thread>

#include "transfer/ServerSession.hpp"

ServerSession::ServerSession()
 : endpoint_{}, server_address_{}, current_client_address_{}, state_(SessionState::OFF) {}

void ServerSession::bind_server(int port) {
    SocketAddress addr = SocketAddress::any(port);
    endpoint_.bind_endpoint(addr);
}

void ServerSession::start() {
    endpoint_.start_receiver();
    state_ = SessionState::IDLE;
}

void ServerSession::stop() {
    endpoint_.stop_receiver();
    state_ = SessionState::OFF;
}

Packet ServerSession::front() {
    return endpoint_.get_front_packet();
}

void ServerSession::manage_packet() {
    Packet packet = endpoint_.get_front_packet();
    MessageType mt = packet.get_header().type_;
    switch (this->state_) {
        case SessionState::OFF:
            handle_off(packet);
            break;
        case SessionState::IDLE:
            handle_idle(packet);
            break;
        case SessionState::CONNECTING:
            handle_connecting(packet);
            break;
        case SessionState::CONNECTED:
            handle_connection(packet);
            break;
        case SessionState::TRANSFERRING:
            handle_transferring(packet);
            break;
        case SessionState::CLOSING:
            handle_closing(packet);
            break;
        default:
        break;
    }
}

void ServerSession::handle_off(Packet& packet) {
    std::cout << "This should never happen: message received from " << packet.get_sender_ip() << std::endl;
}

void ServerSession::handle_idle(Packet& packet) {
    std::cout << "Message received from " << packet.get_sender_ip() << std::endl;
}

void ServerSession::handle_connecting(Packet& packet) {

}

void ServerSession::handle_connection(Packet& packet) {

}

void ServerSession::handle_transferring(Packet& packet) {

}

void ServerSession::handle_closing(Packet& packet) {

}