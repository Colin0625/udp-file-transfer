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
    if (packet.get_message_type() == MessageType::SYN) {
        std::cout << "Received connection request from " << packet.get_sender_ip() << std::endl;
        state_ = SessionState::CONNECTING;
        current_client_address_ = packet.get_sender_addr();
        ssize_t sent = endpoint_.send_packet(Packet(MessageType::SYNACK), current_client_address_);
        std::cout << "Send " << sent << " bytes as connection acceptance to " << packet.get_sender_ip() << std::endl;
    }
    else if (packet.get_message_type() == MessageType::CLOSE) {
        
    }
    else {
        std::cout << "Received invalid packet for idle server from " << packet.get_sender_ip() << std::endl;
    }
}

void ServerSession::handle_connecting(Packet& packet) {
    if (packet.get_message_type() == MessageType::ACK) {
        std::cout << packet.get_sender_ip() << " completed connection, now connected to client" << std::endl;
        state_ = SessionState::CONNECTED;
    }
    else {
        if (packet.get_sender_ip() != current_client_address_.get_ip()) {
            std::cout << "Received message from " << packet.get_sender_ip() << ", disregarding" << std::endl;
            return;
        }
        std::cout << packet.get_sender_ip() << " did not complete connection, resetting state" << std::endl;
        current_client_address_ = SocketAddress{};
        state_ = SessionState::IDLE;
    }
}

void ServerSession::handle_connection(Packet& packet) {
    if (packet.get_message_type() == MessageType::GET) {
        const std::vector<std::byte>& payload = packet.get_payload();
        std::cout << "Payload contents: ";
        std::cout << "size: " << payload.size() << " > ";
        for (const std::byte& b : payload) {
            std::cout << static_cast<char>(b);
        }
        std::cout << std::endl;
        return;
    }
}

void ServerSession::handle_metatransfer(Packet& packet) {

}

void ServerSession::handle_transferring(Packet& packet) {

}

void ServerSession::handle_closing(Packet& packet) {

}