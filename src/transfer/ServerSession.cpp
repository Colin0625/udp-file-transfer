#include <thread>
#include <string>
#include <chrono>

#include "transfer/ServerSession.hpp"

ServerSession::ServerSession()
 : endpoint_{}, server_address_{}, current_client_address_{}, state_(SessionState::OFF), current_requested_file_{}, total_file_packets_{} {}

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
        case SessionState::METATRANSFER:
            handle_metatransfer(packet);
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
        packet.print();
        std::string file_name{};
        for (const std::byte& b : payload) {
            file_name.push_back(static_cast<char>(b));
        }
        std::cout << file_name << std::endl;

        std::filesystem::path file_path = file_name;
        std::ifstream file_stream;
        bool found = find_file(file_path, file_stream);
        if (found) {
            std::cout << file_name << " found!" << std::endl;
        }
        else {
            std::cout << file_name << " not found!" << std::endl;
            ssize_t sent = endpoint_.send_packet(Packet(MessageType::ERROR), current_client_address_);
            std::cout << "Sent " << sent << " bytes as error message to client" << std::endl;
            return;
        }

        Packet metadata_packet = make_metadata_packet(file_path);
        current_requested_file_ = std::move(file_stream);
        state_ = SessionState::TRANSFERRING;
        ssize_t sent = endpoint_.send_packet(metadata_packet, current_client_address_);
        std::cout << "Sent " << sent << " bytes to client as metadata packet" << std::endl;
        return;
    }
}

void ServerSession::handle_metatransfer(Packet& packet) {
    
}

void ServerSession::handle_transferring(Packet& packet) {
    if (packet.get_message_type() == MessageType::ACK) {
        std::cout << "Received ACK packet, ready to transfer file!" << std::endl;
        send_file();
    }
}

void ServerSession::handle_closing(Packet& packet) {

}

bool ServerSession::find_file(const std::filesystem::path& path, std::ifstream& file) {
    std::ifstream tmp_file(path, std::ios::binary);
    if (!tmp_file) {
        return false;
    }
    file = std::move(tmp_file);
    return true;
}

Packet ServerSession::make_metadata_packet(const std::filesystem::path& path) {
    uint64_t file_size = std::filesystem::file_size(path);
    total_file_packets_ = (file_size + 1024 - 1) / 1024;
    std::cout << "File size: " << file_size << std::endl;
    std::cout << "Packets to send: " << total_file_packets_ << std::endl;
    std::vector<std::byte> bytes(8);
    for (int i = 0; i < 8; ++i) {
        bytes[7 - i] = static_cast<std::byte>(file_size & 0xFF);
        file_size >>= 8;
    }

    Packet p(MessageType::METADATA, server_address_, bytes);
    return p;
}

void ServerSession::send_file() {
    constexpr size_t CHUNK_SIZE = 1024;

    std::vector<std::byte> buffer(CHUNK_SIZE);
    uint64_t seq = 0;
    while (true) {
        current_requested_file_.read(reinterpret_cast<char*>(buffer.data()), CHUNK_SIZE);
        size_t bytes_read = current_requested_file_.gcount();

        if (bytes_read == 0) break;
        std::vector<std::byte> payload(buffer.begin(), buffer.begin() + bytes_read);
        Packet p(MessageType::DATA, seq, payload);
        ssize_t sent = endpoint_.send_packet(p, current_client_address_);
        std::cout << "Sent packet " << seq << " with " << sent << " bytes" << std::endl;
        seq++;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (bytes_read < CHUNK_SIZE) break;
    }
    std::cout << "Finished sending file, cleaning up" << std::endl;
    current_requested_file_ = std::ifstream{};
    total_file_packets_ = 0;
    state_ = SessionState::CONNECTED;
}