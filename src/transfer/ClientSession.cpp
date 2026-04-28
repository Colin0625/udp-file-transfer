#include <thread>
#include <cstddef>
#include <filesystem>
#include <string>
#include <chrono>

#include "transfer/ClientSession.hpp"
#include "utility/Queue.hpp"
#include "protocol/Packet.hpp"

ClientSession::ClientSession()
 : endpoint_{}, server_address_{}, state_(SessionState::OFF), current_requested_path_{}, current_output_path_{}, current_output_file_{}, expected_packets_{}, transferring(false) {}


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

void ClientSession::handle_off(Packet& packet) {
    std::cout << "This should never happen: message received from " << packet.get_sender_ip() << std::endl;
}

void ClientSession::handle_idle(Packet& packet) {
    std::cout << "Message received in idle state from " << packet.get_sender_ip() << std::endl;
    packet.print();
}

void ClientSession::handle_connecting(Packet& packet) {
    std::cout << "received packet in connecting state" << std::endl;
    if (packet.get_message_type() == MessageType::SYNACK && packet.get_sender_ip() == server_address_.get_ip()) {
        std::cout << "Received connection acceptance from " << server_address_.get_ip() << " (server)" << std::endl;
        ssize_t synack_sent = endpoint_.send_packet(Packet(MessageType::ACK), server_address_);
        state_ = SessionState::CONNECTED;
        std::cout << "Sent " << synack_sent << " bytes to the server as connection completion" << std::endl;
    }
}

void ClientSession::handle_connection(Packet& packet) {
    if (packet.get_message_type() == MessageType::METADATA) {
        std::string output_folder = "test-output";
        std::string output_filename = current_requested_path_.filename();
        current_output_path_ = std::filesystem::path(output_folder) /output_filename;
        
        uint64_t file_size = 0;
        std::vector<std::byte> bytes = packet.get_payload();

        for (int i = 0; i < 8; ++i) {
            file_size <<= 8;
            file_size |= static_cast<uint64_t>(std::to_integer<unsigned char>(bytes[i]));
        }
        std::cout << "File size: " << file_size << std::endl;

        std::filesystem::remove(current_output_path_);
        current_output_file_ = std::ofstream(current_output_path_, std::ios::binary);
        std::filesystem::resize_file(current_output_path_, file_size);
        expected_packets_ = (file_size + 1024 - 1) / 1024;
        std::cout << "Packets to receive: " << expected_packets_ << std::endl;
        if (!current_output_file_) {
            std::cout << "Failed to create output file!" << std::endl;
            current_requested_path_ = std::filesystem::path{};
            current_output_path_ = std::filesystem::path{};
            current_output_file_ = std::ofstream{};
            expected_packets_ = 0;
            state_ = SessionState::CONNECTED;
            return;
        }
        state_ = SessionState::TRANSFERRING;
        ssize_t sent = endpoint_.send_packet(Packet(MessageType::ACK), server_address_);
        std::cout << "Sent an ack packet for confirmation of readiness for file transfer" << std::endl;
        receive_file();
        return;
    }
    else if (packet.get_message_type() == MessageType::ERROR) {
        std::cout << "Server did not find " << current_requested_path_.string() << std::endl;
        current_requested_path_ = std::filesystem::path{};
        current_output_path_ = std::filesystem::path{};
        current_output_file_ = std::ofstream{};
    }
}

void ClientSession::handle_metatransfer(Packet& packet) {

}

void ClientSession::handle_transferring(Packet& packet) {
    if (packet.get_message_type() == MessageType::DATA) {
        uint32_t seq = packet.get_header().sequence_number_;
        uint16_t payload_size = packet.get_header().payload_size_;
        std::cout << "Received packet " << seq << std::endl;



        std::streamoff offset = static_cast<std::streamoff>(seq) * 1024;

        current_output_file_.seekp(offset);

        if (!current_output_file_) {
            throw std::runtime_error("seekp failed");
        }

        current_output_file_.write(
            reinterpret_cast<const char*>(packet.get_payload().data()),
            payload_size
        );
        if (!current_output_file_) {
            throw std::runtime_error("write failed");
        }
        std::cout << "Finished writing packet " << seq << std::endl;
        if (seq == expected_packets_ - 1) {
            transferring = false;
        }
    }
}

void ClientSession::handle_closing(Packet& packet) {

}

void ClientSession::connect_to_server() {
    ssize_t syn_sent = endpoint_.send_packet(Packet(MessageType::SYN), server_address_);
    std::cout << "Sent " << syn_sent << " bytes to server as connection request" << std::endl;
    state_ = SessionState::CONNECTING;
    manage_packet();
}

void ClientSession::request_file(const std::filesystem::path& file_path) {
    if (state_ != SessionState::CONNECTED) {
        std::cout << "Not connected to server, cannot request" << std::endl;
        return;
    }
    std::string file_string = file_path.string();
    Packet p(MessageType::GET, server_address_, std::span<std::byte>(reinterpret_cast<std::byte*>(file_string.data()), file_string.size()));
    p.print();
    ssize_t sent = endpoint_.send_packet(p, server_address_);
    std::cout << "Sent " << sent << " bytes to server requesting " << file_string << std::endl;
    current_requested_path_ = file_path;
}

void ClientSession::receive_file() {
    transferring = true;
    while (transferring) {
        manage_packet();
    }
    current_output_file_.flush();
    std::cout << "Finished receiving packets" << std::endl;
}