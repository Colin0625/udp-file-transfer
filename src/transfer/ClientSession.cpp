#include <thread>

#include "transfer/ClientSession.hpp"
#include "utility/Queue.hpp"
#include "protocol/Packet.hpp"

ClientSession::ClientSession()
 : endpoint_{}, server_address_{}, state_(SessionState::OFF) {}


void ClientSession::set_server_address(const SocketAddress& addr) {
    server_address_ = addr;
}

void ClientSession::start() {
    endpoint_.start_receiver();
}

void ClientSession::stop() {
    endpoint_.stop_receiver();
}

ssize_t ClientSession::send(Packet p, SocketAddress addr) {
    return endpoint_.send_packet(p, addr);
}

Packet ClientSession::front() {
    return endpoint_.get_front_packet();
}

void ClientSession::manage_packet() {
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

void ClientSession::handle_off(Packet& packet) {
    std::cout << "This should never happen: message received from " << packet.get_sender_ip() << std::endl;
}

void ClientSession::handle_idle(Packet& packet) {
    std::cout << "Message received in idle state from " << packet.get_sender_ip() << std::endl;
    packet.print();
}

void ClientSession::handle_connecting(Packet& packet) {
    if (packet.get_message_type() == MessageType::SYNACK && packet.get_sender_ip() == server_address_.get_ip()) {
        std::cout << "Received connection acceptance from " << server_address_.get_ip() << " (server)" << std::endl;
        ssize_t synack_sent = endpoint_.send_packet(Packet(MessageType::ACK), server_address_);
        state_ = SessionState::CONNECTED;
        std::cout << "Sent " << synack_sent << " bytes to the server as connection completion" << std::endl;
    }
}

void ClientSession::handle_connection(Packet& packet) {

}

void ClientSession::handle_transferring(Packet& packet) {

}

void ClientSession::handle_closing(Packet& packet) {

}


void ClientSession::connect_to_server() {
    ssize_t syn_sent = endpoint_.send_packet(Packet(MessageType::SYN), server_address_);
    std::cout << "Sent " << syn_sent << " bytes to server as connection request" << std::endl;
    state_ = SessionState::CONNECTING;
    manage_packet();
}
